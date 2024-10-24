// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UBulletCountWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* CurrentBulletCount;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* MaxBulletCount;
	UFUNCTION()
	void Update(int NewCurrentBulletCount) const;
	UFUNCTION()
	void SetMaxBulletCount(int NewMaxBulletCount) const;
};
