// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h" //needed for level loading function

void USettingsMenu::NativeConstruct()
{
	//HideMenu(); //menu is hidden immedietly
	BackButton->OnClicked.AddDynamic(this, &USettingsMenu::HideMenu);
	//BackButton->OnClicked.AddDynamic(this, RemoveFromParent);

}

void USettingsMenu::HideMenu()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	
}

void USettingsMenu::ShowMenu()
{
	this->SetVisibility(ESlateVisibility::Visible);

}

void USettingsMenu::returnToMainMenu()
{

}
