// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerState1v1.h"

#include "GameHUD.h"
#include "ScoreCounter.h"
#include "GameFramework/GameStateBase.h"

void APlayerState1v1::OnRep_Score()
{
	Super::OnRep_Score();
	UE_LOG(LogTemp, Display, TEXT("OnRep_Score"));
	//Update the score display.
	if (auto GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		GameHUD->ScoreCounter->UpdateScore(GetWorld()->GetGameState()->PlayerArray[0].Get()->GetScore(), GetWorld()->GetGameState()->PlayerArray[1].Get()->GetScore());
	}
}
