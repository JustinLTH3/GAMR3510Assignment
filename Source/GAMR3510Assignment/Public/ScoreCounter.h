// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreCounter.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UScoreCounter : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* p1Score; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* p2Score; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* p1Name;  

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* p2Name; 

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TimerText;

	void UpdateScore(int p1Points, int p2Points);

	void DisplayPlayerNames(FString name1, FString name2);

	void TimerCountdown(int time);
};
