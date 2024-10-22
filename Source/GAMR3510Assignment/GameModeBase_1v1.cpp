// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase_1v1.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"

AGameModeBase_1v1::AGameModeBase_1v1()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Game/GAMR3510Assignment/BP_MyCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AGameModeBase_1v1::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GameMode Has Started!!"));
}
