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
	//CurrentBulletCount is binded to the umg text for bullets
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* CurrentBulletCount; 

	//MaxBulletCount is binden to the umg text to show the max bullets of the weapon
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* MaxBulletCount; 
	UFUNCTION()

	//updating bullet count when player fires/reloads
	void Update(int NewCurrentBulletCount) const;
	UFUNCTION()

	//setting max bullet count for the weapon
	void SetMaxBulletCount(int NewMaxBulletCount) const; 
};
