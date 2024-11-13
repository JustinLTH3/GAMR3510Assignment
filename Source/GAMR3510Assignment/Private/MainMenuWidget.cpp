// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "SettingsMenu.h"

#include "Kismet/GameplayStatics.h" //needed for level loading function

void UMainMenuWidget::NativeConstruct()
{
	//Adding "QuitGame" function to Quit Button when clicked
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

	//Adding "OpenSettingsMenu" function to Settings button when clicked
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenSettingsMenu);

	//name of the variable, creating a widget of class USettingsMenu, with player controller, and is the same as the bp settings menu
	SettingsMenu = CreateWidget<USettingsMenu>(GetOwningPlayer(), SettingsMenuClass);

	//adding settings menu to viewport
	SettingsMenu->AddToViewport();

	//Adding "ShowMenu" function to SettingsMenu button when clicked
	SettingsMenu->BackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ShowMenu);

	//Hiding Settings menu when game start
	SettingsMenu->HideMenu();
}

void UMainMenuWidget::QuitGame()
{
	//quiting game function
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::HideMenu()
{
	//this (refers to itself which is the widget)
	this->SetVisibility(ESlateVisibility::Collapsed); 
	//set visibility to be hidden and not take space in the scene
}

void UMainMenuWidget::ShowMenu()
{
	//this (refers to itself which is the widget)
	this->SetVisibility(ESlateVisibility::Visible); 
	//set visibility to be visible 
}

void UMainMenuWidget::OpenSettingsMenu()
{
	//opening settings menu hides the main menu
	HideMenu();

	//settings menu is now visible
	SettingsMenu->SetVisibility(ESlateVisibility::Visible);
}
