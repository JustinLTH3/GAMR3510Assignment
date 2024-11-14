// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenu.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API USettingsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	//biding functions and variables to settings menu widget
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

	//Function that makes selected menu invisible
	UFUNCTION(BlueprintCallable)
	void HideMenu();

	//Function that makes selected menu visible
	UFUNCTION()
	void ShowMenu();

};
