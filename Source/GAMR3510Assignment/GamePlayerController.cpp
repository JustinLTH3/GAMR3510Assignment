// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenu.h"
#include "Components/Button.h"

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

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
	//If settings menu is created and is displaying, hide that.
	if (SettingsMenu && SettingsMenu->IsVisible())
	{
		SettingsMenu->HideMenu();
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
		return;
	}
	//set up the settings menu if it is not created.
	if (!SettingsMenu)
	{
		SettingsMenu = CreateWidget<USettingsMenu>(this, SettingsMenuClass);
		SettingsMenu->BackButton->OnClicked.AddUniqueDynamic(this, &AGamePlayerController::OnInputPause);
		SettingsMenu->AddToViewport();
	}
	//Show Menu.
	SetInputMode(FInputModeGameAndUI());
	SettingsMenu->ShowMenu();
	SetShowMouseCursor(true);
}
