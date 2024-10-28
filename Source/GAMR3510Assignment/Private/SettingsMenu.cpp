// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"

void USettingsMenu::NativeConstruct()
{
	HideMenu(); //menu is hidden immedietly
	BackButton->OnClicked.AddDynamic(this, &USettingsMenu::HideMenu);

}

void USettingsMenu::HideMenu()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	
}

void USettingsMenu::ShowMenu()
{
	this->SetVisibility(ESlateVisibility::Visible);
}
