// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"

void UMainMenuWidget::NativeConstruct()
{
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame); //binding quit game function to button
}

void UMainMenuWidget::QuitGame() {

	UE_LOG(LogTemp,Warning,TEXT("Quit"))
}

