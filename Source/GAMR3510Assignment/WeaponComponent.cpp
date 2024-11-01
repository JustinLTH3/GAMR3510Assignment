// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponComponent.h"

#include "BulletCountWidget.h"
#include "GameHUD.h"
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
	OnRep_BulletCount();
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, [this]() { bCanFire = true; }, FireRate, false);
	UE_LOG(LogTemp, Warning, TEXT("%s Shoot"), *GetOwner()->GetName())
	FHitResult Hit;
	UCameraComponent* Cam = Cast<AMyCharacter>(GetOwner())->GetCameraComponent();
	FCollisionObjectQueryParams params;
	GetWorld()->LineTraceSingleByChannel(Hit, Cam->GetComponentLocation(), Cam->GetForwardVector() * 10000 + Cam->GetComponentLocation(), ECC_Visibility);
	if (!Hit.GetActor()) return;
	AActor* HitActor = Hit.GetActor();
	const auto HealthComp = HitActor->GetComponentByClass<UHealthComponent>();
	UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetName());
	if (!HealthComp) return;
	if (Hit.BoneName == FName("Head")) UGameplayStatics::ApplyDamage(HitActor, Damage * 2, Cast<ACharacter>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
	else UGameplayStatics::ApplyDamage(HitActor, Damage, Cast<ACharacter>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
}

bool UWeaponComponent::GetCanFire()
{
	return bCanFire;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MagSize;
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (!Character->GetController()) return;
		const auto Controller = Cast<APlayerController>(Character->GetController());
		if (!Controller) return;
		if (!Controller->GetHUD()) return;
		const auto HUD = Cast<AGameHUD>(Controller->GetHUD());
		if (!HUD->BulletCountWidget) return;
		HUD->BulletCountWidget->Update(BulletCount);
		HUD->BulletCountWidget->SetMaxBulletCount(BulletCount);
	}
}

void UWeaponComponent::OnRep_BulletCount()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (!Character->GetController()) return;
		const auto Controller = Cast<APlayerController>(Character->GetController());
		if (!Controller) return;
		if (!Controller->GetHUD()) return;
		const auto HUD = Cast<AGameHUD>(Controller->GetHUD());
		if (!HUD->BulletCountWidget) return;
		HUD->BulletCountWidget->Update(BulletCount);
	}
}

void UWeaponComponent::ReloadWeapon()
{
	if (bIsReloading) return;
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, [this]()
	{
		bIsReloading = false;
		BulletCount = MagSize;
		OnRep_BulletCount();
	}, ReloadTime, false);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, bCanFire);
	DOREPLIFETIME(UWeaponComponent, BulletCount);
}
