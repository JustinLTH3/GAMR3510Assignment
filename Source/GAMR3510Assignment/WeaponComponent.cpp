// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "HealthComponent.h"
#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
}

void UWeaponComponent::Shoot()
{
	if (!bCanFire) return;
	if (bIsReloading) return;
	if (BulletCount == 0)
	{
		ReloadWeapon();
		return;
	}
	bCanFire = false;
	BulletCount--;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, [this]() { bCanFire = true; }, FireRate, false);
	UE_LOG(LogTemp, Warning, TEXT("%s Shoot"), *GetOwner()->GetName())
	FHitResult Hit;
	UCameraComponent* Cam = Cast<AMyCharacter>(GetOwner())->GetCameraComponent();
	GetWorld()->LineTraceSingleByChannel(Hit, Cam->GetComponentLocation(), Cam->GetForwardVector() * 1000 + Cam->GetComponentLocation(), ECC_Pawn);

	if (!Hit.GetActor()) return;

	AActor* HitActor = Hit.GetActor();
	const auto HealthComp = HitActor->GetComponentByClass<UHealthComponent>();
	UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetName());
	if (!HealthComp) return;
	UGameplayStatics::ApplyDamage(HitActor, Damage, Cast<ACharacter>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
}

bool UWeaponComponent::GetCanFire()
{
	return bCanFire;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MagSize;
}

void UWeaponComponent::ReloadWeapon()
{
	if (bIsReloading) return;
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, [this]()
	{
		bIsReloading = false;
		BulletCount = MagSize;
	}, ReloadTime, false);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, bCanFire);
	DOREPLIFETIME(UWeaponComponent, BulletCount);
}
