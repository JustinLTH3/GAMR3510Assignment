// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameHUD.h"
#include "InputMappingContext.h"
#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"

// Sets default values
AMyCharacter::AMyCharacter()
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
	WeaponComponent->SetupAttachment(GetMesh(),FName("hand_r"));

	CameraComp->bUsePawnControlRotation = false;
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

float AMyCharacter::GetHealth() const
{
	return HealthComp->GetHealth();
}

UCameraComponent* AMyCharacter::GetCameraComponent() const
{
	return CameraComp;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->DieDelegate.AddDynamic(this, &AMyCharacter::OnDie);
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->GetUserSettings()->RegisterInputMappingContext(DefaultMappingContext);
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComp->TakeDamage(ActualDamage);
	return ActualDamage;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyCharacter::Fire);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &AMyCharacter::ServerReloadRPC);
	}
}

void AMyCharacter::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AMyCharacter::OnDie(AActor* Actor)
{
	MulticastOnDieRPC(Actor);
}

void AMyCharacter::Fire()
{
	if (WeaponComponent->GetCanFire())
	{
		FHitResult Hit;
		bool bHitSuccessful = false;

		const FVector2D MidPoint = GEngine->GameViewport->Viewport->GetSizeXY() / 2;
		bHitSuccessful = Cast<APlayerController>(GetController())->GetHitResultAtScreenPosition(MidPoint, ECollisionChannel::ECC_Pawn, true, Hit);
		ServerFireRPC();
	}
}

void AMyCharacter::ServerReloadRPC_Implementation()
{
	if (WeaponComponent) WeaponComponent->ReloadWeapon();
}

void AMyCharacter::ServerFireRPC_Implementation()
{
	if (WeaponComponent) WeaponComponent->Shoot();
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(-Input.Y);
}

void AMyCharacter::MulticastOnDieRPC_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Die"), *GetName())
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(DefaultMappingContext);
	}
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::PossessedBy"));
	OnPossessed(NewController);
}

void AMyCharacter::OnPossessed_Implementation(AController* NewController)
{
	const APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (!PlayerController) return;
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->GetUserSettings()->RegisterInputMappingContext(DefaultMappingContext);
		if (PlayerController->GetHUD())
		{
			AGameHUD* HUD = Cast<AGameHUD>(PlayerController->GetHUD());
			if (!HUD) return;
			HUD->Init(GetHealth(), HealthComp->GetMaxHealth(), WeaponComponent->GetBulletCount());
		}
	}
}
