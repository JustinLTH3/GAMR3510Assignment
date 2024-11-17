// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState1v1.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API APlayerState1v1 : public APlayerState
{
	GENERATED_BODY()
	virtual void OnRep_Score() override;
    //Remove functionality for the score counter to be work properly.
	virtual void Reset() override;
};
