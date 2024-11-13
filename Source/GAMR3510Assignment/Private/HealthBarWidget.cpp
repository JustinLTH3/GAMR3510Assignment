// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	//calculating percentage of max health
	CurrentHealth = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);

	//setting current health
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth);
	}

	//displaying current health as percentage text
	if (HealthText)
	{
		HealthText->SetText(FText::AsPercent(HealthBar->GetPercent()));
	}
}
