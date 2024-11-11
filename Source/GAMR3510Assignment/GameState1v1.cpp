// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState1v1.h"

#include "GameHUD.h"
#include "ScoreCounter.h"
#include "Net/UnrealNetwork.h"

void AGameState1v1::OnRep_RoundTimeLeft()
{
	Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ScoreCounter->TimerCountdown(RoundTimeLeft);
}

void AGameState1v1::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameState1v1, RoundTimeLeft)
}
