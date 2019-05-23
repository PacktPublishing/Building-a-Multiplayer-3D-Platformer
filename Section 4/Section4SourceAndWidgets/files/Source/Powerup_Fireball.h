// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Powerup.h"
#include "Powerup_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APowerup_Fireball : public APowerup
{
	GENERATED_BODY()
	
public:
	
	APowerup_Fireball();

	virtual void UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection) override;
	
};
