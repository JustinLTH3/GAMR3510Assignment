// Fill out your copyright notice in the Description page of Project Settings.
//
#include "GameHUD.h"

#include "PlayerState1v1.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameStateBase.h"
#include "Public/HealthBarWidget.h"
#include "Public/BulletCountWidget.h"
#include "Public/ScoreCounter.h"

AGameHUD::AGameHUD()
	: HealthBar(nullptr)
{
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	HealthBar = CreateWidget<UHealthBarWidget>(GetOwningPlayerController(), HealthBarClass);
	HealthBar->AddToViewport();
	BulletCountWidget = CreateWidget<UBulletCountWidget>(GetOwningPlayerController(), BulletCountClass);
	BulletCountWidget->AddToViewport();

	ScoreCounter = CreateWidget<UScoreCounter>(GetOwningPlayerController(), ScoreCounterClass);
	ScoreCounter->AddToViewport();
}

void AGameHUD::Init(const float Health, const float MaxHealth, const int BulletCount) const
{
	UE_LOG(LogTemp, Warning, TEXT("AGameHUD::Init"));
	if (GetWorld()->GetGameState()->PlayerArray.Num() < 2)
	{
		GetWorldTimerManager().SetTimerForNextTick([this,Health,MaxHealth,BulletCount]() { Init(Health, MaxHealth, BulletCount); });
		return;
	}

	//Initialize the value of the HUD.
	HealthBar->UpdateHealth(Health, MaxHealth);
	BulletCountWidget->SetMaxBulletCount(BulletCount);
	BulletCountWidget->Update(BulletCount);

	ScoreCounter->UpdateScore(GetWorld()->GetGameState()->PlayerArray[0].Get()->GetScore(), GetWorld()->GetGameState()->PlayerArray[1].Get()->GetScore());
	ScoreCounter->DisplayPlayerNames(GetWorld()->GetGameState()->PlayerArray[0].Get()->GetPlayerName(), GetWorld()->GetGameState()->PlayerArray[1].Get()->GetPlayerName());
}
