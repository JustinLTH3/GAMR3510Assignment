// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_1v1.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"


AGameMode_1v1::AGameMode_1v1()
{

}

bool AGameMode_1v1::ReadyToStartMatch_Implementation()
{
	if (NumPlayers == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("ready!!"));
		return true;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("not ready!!"));

		return false;
	}

}

void AGameMode_1v1::ResetLevel()
{
	Super::ResetLevel();

	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		RestartPlayer(X->Get());
	}

}

void AGameMode_1v1::BeginPlay()
{
	Super::BeginPlay();
	ReadyToStartMatch_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("GameMode Has Started!!"));
}
