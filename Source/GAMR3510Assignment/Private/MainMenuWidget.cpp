// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "SettingsMenu.h"

#include "Kismet/GameplayStatics.h" //needed for level loading function

void UMainMenuWidget::NativeConstruct()
{
	Multiplayer->OnClicked.AddDynamic(this, &UMainMenuWidget::StartGame);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame); //binding quit game function to button
	Settings->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSettingsMenu);
	SettingsMenu = CreateWidget<USettingsMenu>(GetOwningPlayer(), SettingsMenuClass);//name of the variable, creating a widget of class USettingsMenu, with player controller, and is the same as the bp settings menu
	SettingsMenu->AddToViewport();
	SettingsMenu->BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowMenu);
}

void UMainMenuWidget::QuitGame() {

	UE_LOG(LogTemp, Warning, TEXT("Quit"))
}

void UMainMenuWidget::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Started"))

		FName LevelName = "UI_Testing";  // The name of the level you want to load
	UGameplayStatics::OpenLevel(this, LevelName);
}

void UMainMenuWidget::HideMenu()
{
	this->SetVisibility(ESlateVisibility::Collapsed); //this (refers to itself which is the widget)
	//set visibility to be hidden and not take space in the scene
}

void UMainMenuWidget::ShowMenu()
{
	this->SetVisibility(ESlateVisibility::Visible); //this (refers to itself which is the widget)
	//set visibility to be visible 
}

void UMainMenuWidget::OpenSettingsMenu()
{
	HideMenu();
	SettingsMenu->SetVisibility(ESlateVisibility::Visible);
}



