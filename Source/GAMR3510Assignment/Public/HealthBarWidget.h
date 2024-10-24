// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMR3510ASSIGNMENT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // The progress bar that shows the health
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) // Binding this with the widget that has the same name
        class UProgressBar* HealthBar;  // Progress bar

    // The text block that shows the percentage
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        class UTextBlock* HealthText;  // Text that shows health

    // Function to update both the progress bar and text
    void UpdateHealth(float CurrentHealth, float MaxHealth);
    
};
