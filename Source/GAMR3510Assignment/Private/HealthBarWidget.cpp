// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBarWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("function has been called"));
	CurrentHealth = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);

	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::AsPercent(HealthBar->GetPercent()));
		// HealthAmount->SetText(FText::AsNumber(CurrentHealth));
	}
}
