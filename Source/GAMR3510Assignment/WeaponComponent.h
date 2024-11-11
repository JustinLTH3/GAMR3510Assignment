// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMR3510ASSIGNMENT_API UWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();
	UFUNCTION()
	bool GetCanFire();
	UFUNCTION()
	int GetBulletCount() const { return BulletCount; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage = 10;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRate = .2;
	UPROPERTY()
	FTimerHandle FireRateTimerHandle;
	UPROPERTY(Replicated)
	bool bCanFire = true;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	int MagSize = 10;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float ReloadTime = 2;
	bool bIsReloading = false;
	UPROPERTY()
	FTimerHandle ReloadTimerHandle;
	UPROPERTY(ReplicatedUsing=OnRep_BulletCount)
	int BulletCount;
	UFUNCTION()
	void OnRep_BulletCount();
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* FireEffectMuzzle;

public:
	UFUNCTION()
	void ReloadWeapon();
	UFUNCTION()
	void Shoot();
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastFire(const FVector& Start, const FVector& End);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
