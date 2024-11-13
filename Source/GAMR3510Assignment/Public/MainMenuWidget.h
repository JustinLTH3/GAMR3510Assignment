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
	//constructs the user widget and used for applying values to variable and functions
	virtual void NativeConstruct() override; 

	//using "UPROPERTY(BlueprintReadWrite, meta = (BindWidget))" to bind widgets with c++
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) 
	class UButton* SettingsButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USettingsMenu* SettingsMenu;

	//make it appear in editor
	UPROPERTY(EditDefaultsOnly) 
	TSubclassOf<USettingsMenu> SettingsMenuClass;

	//need it to bind functions
	//quit game function
	UFUNCTION() 
	void QuitGame();

	//hide menu function
	UFUNCTION()
	void HideMenu();

	//show menu function, can be called within blueprint
	UFUNCTION(BlueprintCallable)
	void ShowMenu();

	//open settins function
	UFUNCTION()
	void OpenSettingsMenu();
};
