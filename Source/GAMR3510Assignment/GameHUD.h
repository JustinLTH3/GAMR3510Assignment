// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UBulletCountWidget;
class UHealthBarWidget;
class UScoreCounter;
/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	UPROPERTY(BlueprintReadWrite)
	UScoreCounter* ScoreCounter;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UScoreCounter> ScoreCounterClass;

	UPROPERTY(BlueprintReadWrite)
	UHealthBarWidget* HealthBar;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBarWidget> HealthBarClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBulletCountWidget> BulletCountClass;
	UPROPERTY(BlueprintReadWrite)
	UBulletCountWidget* BulletCountWidget;
	UFUNCTION()
	void Init(float Health, float MaxHealth, int BulletCount) const;
};
