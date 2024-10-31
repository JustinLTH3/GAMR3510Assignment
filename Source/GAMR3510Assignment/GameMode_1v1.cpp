// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_1v1.h"

#include "HealthComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

AGameMode_1v1::AGameMode_1v1()
{
}

bool AGameMode_1v1::ReadyToStartMatch_Implementation()
{
	if (NumPlayers == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Match ready!!"));
		for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
		{
			//Check each client has loaded the world before the match is start.
			if (!X->Get()->HasClientLoadedCurrentWorld()) return false;
		}
		return true;
	}
	return false;
}

void AGameMode_1v1::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	NewPlayer->GetCharacter()->GetComponentByClass<UHealthComponent>()->DieDelegate.AddUniqueDynamic(this, &AGameMode_1v1::OnPlayerDie);
}

void AGameMode_1v1::ResetLevel()
{
	Super::ResetLevel();
	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		RestartPlayer(X->Get());
	}
}

void AGameMode_1v1::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerDeathCount.Add(NewPlayer, 0);
}

void AGameMode_1v1::OnPlayerDie(AActor* Actor)
{
	if (auto x = Cast<ACharacter>(Actor))
	{
		//Add Score. If the score reached, end game. Else restart level.
		PlayerDeathCount[x->GetController()]++;
		if (PlayerDeathCount[x->GetController()] == 5) EndMatch();
		else ResetLevel();
	}
}
