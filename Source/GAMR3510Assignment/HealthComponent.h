// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthComponentDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMR3510ASSIGNMENT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	void TakeDamage(float Damage);
	UPROPERTY() //Broadcast when the owner of the component die.
	FHealthComponentDelegate DieDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float health = 100;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
UFUNCTION()
	void OnRep_Health();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
