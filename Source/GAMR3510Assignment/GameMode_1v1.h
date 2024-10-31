// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMode_1v1.generated.h"

class AMyCharacter;
/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API AGameMode_1v1 : public AGameMode
{
	GENERATED_BODY()

public:
	AGameMode_1v1();

	UPROPERTY()
	TMap<AController*, int> PlayerDeathCount;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void ResetLevel() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
protected:
	virtual void RestartPlayer(AController* NewPlayer) override;
	UFUNCTION()
	void OnPlayerDie(AActor* Actor);
};
