// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_1v1.h"

#include "GameState1v1.h"
#include "HealthComponent.h"
#include "MyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

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

void AGameMode_1v1::RoundTimeRunOut()
{
	GetWorld()->GetTimerManager().SetTimer(BleedingTimer, this, &AGameMode_1v1::PlayerBleeding, 1, true);
}

void AGameMode_1v1::RoundTimeUpdate()
{
	Cast<AGameState1v1>(GameState)->SetRoundTimeLeft(GetWorldTimerManager().TimerExists(RoundTimerHandle) ? GetWorldTimerManager().GetTimerRemaining(RoundTimerHandle) : 0);
	if (!GetWorldTimerManager().TimerExists(RoundTimerHandle)) GetWorldTimerManager().ClearTimer(RoundTimeUpdateHandle);
}

void AGameMode_1v1::PlayerBleeding()
{
	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		UGameplayStatics::ApplyDamage(X->Get()->GetCharacter(), 5, nullptr, nullptr, UDamageType::StaticClass());
	}
}

void AGameMode_1v1::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	NewPlayer->GetCharacter()->GetComponentByClass<UHealthComponent>()->DieDelegate.AddUniqueDynamic(this, &AGameMode_1v1::OnPlayerDie);
}

void AGameMode_1v1::StartMatch()
{
	Super::StartMatch();
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AGameMode_1v1::RoundTimeRunOut, 60);
	GetWorldTimerManager().SetTimer(RoundTimeUpdateHandle, this, &AGameMode_1v1::RoundTimeUpdate, .5f, true);
}

void AGameMode_1v1::EndMatch()
{
	Super::EndMatch();
}

void AGameMode_1v1::ResetLevel()
{
	Super::ResetLevel();
	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		RestartPlayer(X->Get());
	}
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AGameMode_1v1::RoundTimeRunOut, 60);
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
		if (BleedingTimer.IsValid()) GetWorldTimerManager().ClearTimer(BleedingTimer);
		//Add Score. If the score reached, end game. Else restart level.
		PlayerDeathCount[x->GetController()]++;
		for (auto Player : GameState.Get()->PlayerArray)
		{
			if (Player != x->GetPlayerState())
			{
				Player->SetScore(PlayerDeathCount[x->GetController()]);
				Player->OnRep_Score();
			}
		}
		auto y = &AGameMode_1v1::ResetLevel;
		if (PlayerDeathCount[x->GetController()] == 5) y = &AGameMode_1v1::EndMatch;
		if (GetWorldTimerManager().TimerExists(RoundTimerHandle)) GetWorldTimerManager().SetTimer(RoundTimeEndHandle, this, y, 3);
	}
}

//////countdown give both a point when draw
