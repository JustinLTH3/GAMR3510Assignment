// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "GameHUD.h"
#include "HealthBarWidget.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

float UHealthComponent::GetHealth() const
{
	return health;
}

void UHealthComponent::TakeDamage(const float Damage)
{
	health -= Damage;
	if (health <= 0) DieDelegate.Broadcast();
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	health = MaxHealth;
	OnRep_Health();
}

void UHealthComponent::OnRep_Health()
{
	UE_LOG(LogTemp, Display, TEXT("Health Component On Replication"));
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		if (!Character->GetController()) return;
		const auto Controller = Cast<APlayerController>(Character->GetController());
		if (!Controller) return;
		if (!Controller->GetHUD()) return;
		Cast<AGameHUD>(Controller->GetHUD())->HealthBar->UpdateHealth(health, MaxHealth);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, health);
}
