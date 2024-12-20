// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

#include "GameHUD.h"
#include "HealthBarWidget.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

void UHealthComponent::TakeDamage(const float Damage)
{
	if (Health <= 0) return;
	Health -= Damage;
	OnRep_Health();
	if (Health <= 0) DieDelegate.Broadcast(GetOwner());
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	OnRep_Health();
}

void UHealthComponent::OnRep_Health()
{
	UE_LOG(LogTemp, Display, TEXT("Health Component On Replication"));
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		const auto Controller = Cast<APlayerController>(Character->GetController());
		if (!Controller) return;
		if (!Controller->GetHUD()) return;
		Cast<AGameHUD>(Controller->GetHUD())->HealthBar->UpdateHealth(Health, MaxHealth);
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
	DOREPLIFETIME(UHealthComponent, Health);
}
