// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenu.h"
#include "Components/Button.h"

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(Ia_Pause, ETriggerEvent::Started, this, &AGamePlayerController::OnInputPause);
	}
}

void AGamePlayerController::OnInputPause()
{
	UE_LOG(LogTemp, Warning, TEXT("InputPause"));
	if (SettingsMenu && SettingsMenu->IsVisible())
	{
		SettingsMenu->HideMenu();
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		return;
	}
	if (!SettingsMenu)
	{
		SettingsMenu = CreateWidget<USettingsMenu>(this, SettingsMenuClass);
		SettingsMenu->BackButton->OnClicked.AddUniqueDynamic(this, &AGamePlayerController::OnInputPause);
		SettingsMenu->AddToViewport();
	}
	SetInputMode(FInputModeGameAndUI());
	SettingsMenu->ShowMenu();
	SetShowMouseCursor(true);
}
