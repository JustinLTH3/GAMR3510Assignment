// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreCounter.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UScoreCounter : public UUserWidget
{
	GENERATED_BODY()

public:

	//using "UPROPERTY(BlueprintReadWrite, meta = (BindWidget))" to bind widgets with c++

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* p1Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* p2Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* p1Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* p2Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WinnerText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WinText;


	//update score function that takes 2 int variables
	void UpdateScore(int p1Points, int p2Points);

	//DisplayPlayerNames function which takes 2 strings that represent player names
	void DisplayPlayerNames(FString name1, FString name2);

	//TimerCountdown that takes int variable
	void TimerCountdown(int time);
};
