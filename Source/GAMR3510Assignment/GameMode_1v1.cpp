// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_1v1.h"

#include "GameState1v1.h"
#include "HealthComponent.h"
#include "MultiplayerSessionsSubsystem.h"
#include "MyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameSession.h"
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
		if (GetWorld()->GetNumPlayerControllers() == 2)
		{
			for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
			{
				//Check each client has loaded the world before the match is start.
				if (!X->Get()->HasClientLoadedCurrentWorld()) return false;
			}
			UE_LOG(LogTemp, Warning, TEXT("Match ready!!"));
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

AActor* AGameMode_1v1::ChoosePlayerStart_Implementation(AController* Player)
{
	if (!PlayerStart)
	{
		PlayerStart = Super::ChoosePlayerStart_Implementation(Player);
		return PlayerStart;
	}
	AActor* AnotherPlayerStart = Super::ChoosePlayerStart_Implementation(Player);
	while (AnotherPlayerStart == PlayerStart)
	{
		AnotherPlayerStart = Super::ChoosePlayerStart_Implementation(Player);
	}
	PlayerStart = nullptr;
	return AnotherPlayerStart;
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
	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		Cast<AMyCharacter>(X->Get()->GetCharacter())->MulticastDisconnectRPC();
	}
	ReturnToMainMenuHost();
}

void AGameMode_1v1::DecideWinner()
{
	TArray<APlayerState*> Winners;
	for (int i = 0; i < NumPlayers; ++i)
	{
		if (GameState->PlayerArray[i]->GetScore() == 5) Winners.AddUnique(GameState->PlayerArray[i]);
	}
	if (Winners.Num() == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Draw"))
		AGameState1v1* GameState1V1 = GetGameState<AGameState1v1>();
		GameState1V1->bIsDraw = true;
		GameState1V1->OnRep_Winner();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Winner Is %s"), *Winners.Last()->GetPlayerName());
		AGameState1v1* GameState1V1 = GetGameState<AGameState1v1>();
		GameState1V1->Winner = Winners.Last();
		GameState1V1->OnRep_Winner();
	}
	GetWorldTimerManager().SetTimer(RoundTimeEndHandle, this, &AGameMode_1v1::EndMatch, 3);
}

void AGameMode_1v1::ResetLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetLevel"));
	Super::ResetLevel();
	for (auto X = GetWorld()->GetPlayerControllerIterator(); X; ++X)
	{
		RestartPlayer(X->Get());
	}
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &AGameMode_1v1::RoundTimeRunOut, 60);
	GetWorld()->GetTimerManager().SetTimer(RoundTimeUpdateHandle, this, &AGameMode_1v1::RoundTimeUpdate, .5f, true);
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
		UE_LOG(LogTemp, Warning, TEXT("OnPlayerDie Reset"))
		auto func = &AGameMode_1v1::ResetLevel;
		float timer = 4;
		if (PlayerDeathCount[x->GetController()] == 5)
		{
			func = &AGameMode_1v1::DecideWinner;
			timer = 1;
		}
		if (GetWorldTimerManager().TimerExists(RoundTimerHandle)) GetWorldTimerManager().ClearTimer(RoundTimerHandle);
		if (!GetWorldTimerManager().TimerExists(RoundTimeEndHandle)) GetWorldTimerManager().SetTimer(RoundTimeEndHandle, this, func, timer);
	}
}

//////countdown give both a point when draw
