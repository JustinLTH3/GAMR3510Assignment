// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharater.h"

#include "HealthComponent.h"

// Sets default values
AMyCharater::AMyCharater()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	
}

float AMyCharater::GetHealth() const
{
	return HealthComp->GetHealth();
}

// Called when the game starts or when spawned
void AMyCharater::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
