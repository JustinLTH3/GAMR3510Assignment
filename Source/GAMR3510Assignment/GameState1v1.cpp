// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState1v1.h"

#include "GameHUD.h"
#include "MultiplayerSessionsSubsystem.h"
#include "ScoreCounter.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void AGameState1v1::OnRep_RoundTimeLeft()
{
	Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ScoreCounter->TimerCountdown(RoundTimeLeft);
}

void AGameState1v1::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameState1v1, RoundTimeLeft)
	DOREPLIFETIME(AGameState1v1, Winner)
	DOREPLIFETIME(AGameState1v1, bIsDraw)
}

void AGameState1v1::OnRep_Winner()
{
	auto HUD = GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetHUD();
	if (!HUD) return;
	auto GHud = Cast<AGameHUD>(HUD);
	if (!GHud) return;

	if (bIsDraw)
	{
		//Display only "Draw" if is draw.
		GHud->ScoreCounter->WinnerText->SetText(FText::FromString("Draw"));
		GHud->ScoreCounter->WinnerText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//Display Winner.
		GHud->ScoreCounter->WinnerText->SetText(FText::FromString(Winner->GetPlayerName()));
		GHud->ScoreCounter->WinnerText->SetVisibility(ESlateVisibility::Visible);
		GHud->ScoreCounter->WinText->SetVisibility(ESlateVisibility::Visible);
	}
}
