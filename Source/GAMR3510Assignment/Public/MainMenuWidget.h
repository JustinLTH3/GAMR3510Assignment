// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "MainMenuWidget.generated.h"

/**
 *
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UMainMenuWidget : public UMenu
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override; //constructs the user widget and used for applying values to variable and functions

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) //basically if the name of the variable here is the 
	//same as the one in BP then you can access the BP one
	class UButton* Settings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USettingsMenu* SettingsMenu;
	UPROPERTY(EditDefaultsOnly) //make it appear in editor
	TSubclassOf<USettingsMenu> SettingsMenuClass;

	UFUNCTION() //need it to bind functions
	void QuitGame();

	UFUNCTION()
	void HideMenu();

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	UFUNCTION()
	void OpenSettingsMenu();
};
