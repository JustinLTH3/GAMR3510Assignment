// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"

#include "HealthComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameHUD.h"
#include "InputMappingContext.h"
#include "MultiplayerSessionsSubsystem.h"
#include "WeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	WeaponComponent->SetupAttachment(GetMesh(), FName("hand_r_weapon_socket"));

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
		if (PlayerController->GetHUD())
		{
			AGameHUD* HUD = Cast<AGameHUD>(PlayerController->GetHUD());
			if (!HUD) return;
			HUD->Init(GetHealth(), HealthComp->GetMaxHealth(), WeaponComponent->GetBulletCount());
		}
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
		//Also call move function when the player released all the keys so that we can stop the footstep audio.
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &AMyCharacter::Fire);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Started, this, &AMyCharacter::ServerReloadRPC);
	}
}

void AMyCharacter::OnUnPossessed_Implementation()
{
	if (AudioSound)
	{
		AudioSound->Stop();
		AudioSound->DestroyComponent();
		AudioSound = nullptr;
	}
}

void AMyCharacter::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);

	//Play footstep sound on host and client. 
	if (GetWorld()->GetAuthGameMode()) NetMulticastFootStepRPC(MovementVector);
	else ServerFootStepRPC(MovementVector);
}

void AMyCharacter::NetMulticastFootStepRPC_Implementation(FVector2D Input)
{
	if (!AudioSound && !Input.IsZero() && GetCharacterMovement()->IsMovingOnGround())
	{
		AudioSound = UGameplayStatics::SpawnSoundAttached(WalkSound, RootComponent);
		AudioSound->bAutoDestroy = true;
		AudioSound->bAllowSpatialization = true;

		//AudioSound->SetSound(WalkSound);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Walk Sound Works")));
	}
	else if (Input.IsZero() || !GetCharacterMovement()->IsMovingOnGround())
	{
		if (!AudioSound) return;
		AudioSound->DestroyComponent();
		AudioSound->Stop();
		AudioSound = nullptr;
	}
}

void AMyCharacter::ServerFootStepRPC_Implementation(FVector2D Input)
{
	NetMulticastFootStepRPC(Input);
}

void AMyCharacter::OnDie(AActor* Actor)
{
	WeaponComponent->SetSimulatePhysics(true);
	MulticastOnDieRPC(Actor);
}

void AMyCharacter::Fire()
{
	//Only call rpc when the player can shoot to prevent sending too much rpc.
	if (WeaponComponent->GetCanFire())
	{
		ServerFireRPC();
	}
}

void AMyCharacter::MulticastDisconnectRPC_Implementation()
{
	//Destroy the game session to disconnect properly.
	GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>()->DestroySession();
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
	if (AudioSound)
	{
		AudioSound->Stop();
		AudioSound->DestroyComponent();
		AudioSound = nullptr;
	}
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

void AMyCharacter::UnPossessed()
{
	Super::UnPossessed();
	OnUnPossessed();
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
