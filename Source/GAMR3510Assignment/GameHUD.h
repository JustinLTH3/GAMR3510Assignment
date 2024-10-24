// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UBulletCountWidget;
class UHealthBarWidget;
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
	UHealthBarWidget* HealthBar;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealthBarWidget> HealthBarClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBulletCountWidget> BulletCountClass;
	UPROPERTY(BlueprintReadWrite)
	UBulletCountWidget* BulletCountWidget;
};
