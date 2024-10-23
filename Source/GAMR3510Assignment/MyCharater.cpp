// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharater.h"

#include "HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharater::AMyCharater()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComp"));

	ConstructorHelpers::FObjectFinder<UInputMappingContext> CH(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/GAMR3510Assignment/Input/IMC_Default'"));
	DefaultMappingContext = CH.Object;
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400;
	CameraComp->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	WeaponComponent->SetupAttachment(RootComponent);

	CameraComp->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

float AMyCharater::GetHealth() const
{
	return HealthComp->GetHealth();
}

void AMyCharater::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->DieDelegate.AddDynamic(this, &AMyCharater::OnDie);
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMyCharater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMyCharater::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComp->TakeDamage(ActualDamage);
	return ActualDamage;
}

// Called to bind functionality to input
void AMyCharater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharater::Move);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyCharater::Fire);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharater::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharater::Jump);
	}
}

void AMyCharater::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AMyCharater::Fire()
{
	if (WeaponComponent->GetCanFire())
	{
		FHitResult Hit;
		bool bHitSuccessful = false;

		const FVector2D MidPoint = GEngine->GameViewport->Viewport->GetSizeXY() / 2;
		bHitSuccessful = Cast<APlayerController>(GetController())->GetHitResultAtScreenPosition(MidPoint, ECollisionChannel::ECC_Pawn, true, Hit);
		ServerFireRPC(bHitSuccessful, Hit);
	}
}

void AMyCharater::ServerFireRPC_Implementation(bool bIsSuccessful, const FHitResult& HitResult)
{
	if (WeaponComponent) WeaponComponent->Shoot(bIsSuccessful, HitResult);
}

void AMyCharater::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(-Input.Y);
}

void AMyCharater::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("AMyCharater::PossessedBy"));
	const APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (!PlayerController) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMyCharater::UnPossessed()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
	Super::UnPossessed();
}

void AMyCharater::OnDie()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Die"), *GetName())
}
