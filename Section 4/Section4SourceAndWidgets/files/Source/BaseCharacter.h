// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Powerup.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MASTERINGUNREAL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/* GAME ELEMENTS */
	//Array for holding powerups
	TArray<APowerup *> PowerUps;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Base Character")
		bool bIsFiring;

	/* COMPONENTS */
	//Spring Arm for holding camera
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		USpringArmComponent* SpringArm;

	//Camera Component
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UCameraComponent *Camera;

	//Arrow for determining forward direction
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UArrowComponent *TraceDirection;

	/* MOVEMENT/CAMERA INPUT FUNCTIONS */

	void MoveForward(float amount);

	void MoveRight(float amount);

	void RotateCamera(float amount);

	void ChangeCameraHeight(float amount);

	/* PICKUP FUNCTIONS */
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectCoin();

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectHeart();

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectPowerup(APowerup *powerup);

	//Function for cycling between powerups
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void NextPowerup();


	//DAMAGE FUNCTION
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void ReceiveDamage(int amount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* USE POWERUP FUNCTIONS */
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void UsePowerupStartClient();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Base Character")
		void UsePowerupStartServer();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Base Character")
		void PowerupUsed();

private:
	int SelectedPowerupIndex;
	
	
};
