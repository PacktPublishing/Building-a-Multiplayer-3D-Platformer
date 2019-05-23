// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Powerup.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APowerup : public APickup
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power Up")
		UTexture2D *icon;

	//override the collect pickup function
	virtual void CollectPickup_Implementation(ABaseCharacter *Character) override;

	//function for using the powerup
	virtual void UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection);
	
	
};
