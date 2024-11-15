// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	UPROPERTY(EditAnywhere)
	class UInputAction* Ia_Pause;
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* MappingContext;
	UFUNCTION()
	void OnInputPause();
	UPROPERTY()
	class USettingsMenu* SettingsMenu;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingsMenu> SettingsMenuClass;
};
