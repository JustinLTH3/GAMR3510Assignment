// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/KismetSystemLibrary.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 *
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Multiplayer;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Settings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;
	UFUNCTION() //need it to bind functions
	void QuitGame();

};
