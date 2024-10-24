// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMR3510ASSIGNMENT_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();
	UFUNCTION()
	bool GetCanFire();

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
	UPROPERTY(Replicated)
	int BulletCount;

public:
	UFUNCTION()
	void ReloadWeapon();
	UFUNCTION()
	void Shoot();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;
};
