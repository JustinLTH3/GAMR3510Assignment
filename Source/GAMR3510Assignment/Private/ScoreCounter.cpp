// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreCounter.h"
#include "Components/TextBlock.h"

void UScoreCounter::UpdateScore(int p1Points, int p2Points)
{
	p1Score->SetText(FText::AsNumber(p1Points));
	p2Score->SetText(FText::AsNumber(p2Points));
}

void UScoreCounter::DisplayPlayerNames(FString name1, FString name2)
{
	p1Name->SetText(FText::FromString(name1));
	p2Name->SetText(FText::FromString(name2));


}

void UScoreCounter::TimerCountdown(int time)
{
	TimerText->SetText(FText::AsNumber(time));
}