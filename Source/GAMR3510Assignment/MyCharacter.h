// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UHealthComponent;
class USpringArmComponent;
class UWeaponComponent;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;

UCLASS()
class GAMR3510ASSIGNMENT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	UFUNCTION()
	float GetHealth() const;
	UFUNCTION()
	UCameraComponent* GetCameraComponent() const;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* HealthComp;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Fire;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Reload;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	class USoundBase* WalkSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class UAudioComponent* AudioSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	UFUNCTION(NetMulticast, Reliable)
	void OnPossessed(AController* NewController);
	UFUNCTION(NetMulticast, Reliable)
	void OnUnPossessed();
	UFUNCTION()
	void Move(const struct FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION(NetMulticast, Reliable)
	//For making the character a ragdoll.
	void MulticastOnDieRPC(AActor* Actor);
	UFUNCTION(NetMulticast, Reliable)
	//Make the sound to play on both client and host if the owner of this character is the host.
	void NetMulticastFootStepRPC(FVector2D Input);
	UFUNCTION(Server, Reliable)
	//Make the sound to play on the host.
	void ServerFootStepRPC(FVector2D Input);
	UFUNCTION()
	//The callback function bind to health component that called when the player die.
	void OnDie(AActor* Actor);

	UFUNCTION()
	void Fire();
	UFUNCTION(Server, Unreliable)
	//Called on client to perform the hit calculation on host.
	void ServerFireRPC();
	UFUNCTION(Server, Reliable)
	//Called on client to reload on host.
	void ServerReloadRPC();
	UFUNCTION(NetMulticast, Reliable)
	//Make the client disconnect properly.
	void MulticastDisconnectRPC();
};
