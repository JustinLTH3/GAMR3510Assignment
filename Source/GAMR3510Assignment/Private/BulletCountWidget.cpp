// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletCountWidget.h"

#include "Components/TextBlock.h"

//updating dispalying text as number to show bullet count ingame
void UBulletCountWidget::Update(const int NewCurrentBulletCount) const
{
	CurrentBulletCount->SetText(FText::AsNumber(NewCurrentBulletCount));
}

//setting max bullet count displaying text as a number to show bullet count ingame
void UBulletCountWidget::SetMaxBulletCount(const int NewMaxBulletCount) const
{
	MaxBulletCount->SetText(FText::AsNumber(NewMaxBulletCount));
}
