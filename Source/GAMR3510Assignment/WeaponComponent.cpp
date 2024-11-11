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
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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
	FHitResult HitFromCam;
	UCameraComponent* Cam = Cast<AMyCharacter>(GetOwner())->GetCameraComponent();
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	GetWorld()->LineTraceSingleByChannel(HitFromCam, Cam->GetComponentLocation(), Cam->GetForwardVector() * 10000 + Cam->GetComponentLocation(), ECC_Visibility, params);
	if (HitFromCam.GetActor())
	{
		NetMulticastFire(GetSocketLocation(FName("Muzzle")), HitFromCam.Location - GetSocketLocation(FName("Muzzle")));
	}
	else
	{
		NetMulticastFire(GetSocketLocation(FName("Muzzle")), Cam->GetForwardVector() * 10000);
	}
	if (!HitFromCam.GetActor()) return;
	FHitResult HitFromWeapon;
	GetWorld()->LineTraceSingleByChannel(HitFromWeapon, GetSocketLocation(FName("Muzzle")), HitFromCam.Location, ECC_Visibility, params);
	AActor* HitActor;
	if (HitFromWeapon.GetActor()) HitActor = HitFromWeapon.GetActor();
	else HitActor = HitFromCam.GetActor();
	const auto HealthComp = HitActor->GetComponentByClass<UHealthComponent>();
	UE_LOG(LogTemp, Display, TEXT("Hit Actor: %s"), *HitActor->GetName());
	if (!HealthComp) return;
	if (HitFromCam.BoneName == FName("Head")) UGameplayStatics::ApplyDamage(HitActor, Damage * 2, Cast<ACharacter>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
	else UGameplayStatics::ApplyDamage(HitActor, Damage, Cast<ACharacter>(GetOwner())->GetController(), GetOwner(), UDamageType::StaticClass());
}

void UWeaponComponent::NetMulticastFire_Implementation(const FVector& Start, const FVector& End)
{
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FireEffectMuzzle, Start, FRotator(0), FVector(1), true, false);
	NiagaraComp->SetVariableVec3(FName("Beam End"), End);
	NiagaraComp->Activate();
}

bool UWeaponComponent::GetCanFire()
{
	return bCanFire;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BulletCount = MagSize;
	OnRep_BulletCount();
}

void UWeaponComponent::OnRep_BulletCount()
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (!Character->GetController()) return;
	const auto Controller = Cast<APlayerController>(Character->GetController());
	if (!Controller) return;
	if (!Controller->GetHUD()) return;
	const auto HUD = Cast<AGameHUD>(Controller->GetHUD());
	if (!HUD->BulletCountWidget) return;
	HUD->BulletCountWidget->Update(BulletCount);
	HUD->BulletCountWidget->SetMaxBulletCount(MagSize);
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
