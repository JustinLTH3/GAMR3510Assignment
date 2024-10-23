// Fill out your copyright notice in the Description page of Project Settings.

#include "GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Public/HealthBarWidget.h"

AGameHUD::AGameHUD()
	: HealthBar(nullptr)
{
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	HealthBar = CreateWidget<UHealthBarWidget>(GetOwningPlayerController(), HealthBarClass);
	HealthBar->AddToViewport();
}
