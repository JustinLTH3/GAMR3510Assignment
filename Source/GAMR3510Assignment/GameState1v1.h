// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameState1v1.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API AGameState1v1 : public AGameState
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetRoundTimeLeft(int NewValue)
	{
		RoundTimeLeft = NewValue;
		OnRep_RoundTimeLeft();
	}

	UPROPERTY(ReplicatedUsing=OnRep_RoundTimeLeft)
	int RoundTimeLeft;
	UFUNCTION()
	void OnRep_RoundTimeLeft();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
