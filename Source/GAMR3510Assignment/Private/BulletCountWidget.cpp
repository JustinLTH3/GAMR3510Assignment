// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletCountWidget.h"

#include "Components/TextBlock.h"

void UBulletCountWidget::Update(const int NewCurrentBulletCount) const
{
	CurrentBulletCount->SetText(FText::AsNumber(NewCurrentBulletCount));
}

void UBulletCountWidget::SetMaxBulletCount(const int NewMaxBulletCount) const
{
	MaxBulletCount->SetText(FText::AsNumber(NewMaxBulletCount));
}
