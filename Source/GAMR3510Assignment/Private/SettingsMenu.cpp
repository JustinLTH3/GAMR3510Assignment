// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h" //needed for level loading function

void USettingsMenu::NativeConstruct()
{
	//adding "HideMenu" function to Back Button when it is clicked
	BackButton->OnClicked.AddDynamic(this, &USettingsMenu::HideMenu);
}

void USettingsMenu::HideMenu()
{
	//hiding menu
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void USettingsMenu::ShowMenu()
{
	//showing menu
	this->SetVisibility(ESlateVisibility::Visible);

}

void USettingsMenu::returnToMainMenu()
{
	//currently not used/ no functionality
}
