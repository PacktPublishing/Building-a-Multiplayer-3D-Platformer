// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "Pickup_Heart.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APickup_Heart : public APickup
{
	GENERATED_BODY()
	
public:
	APickup_Heart();

	virtual void CollectPickup_Implementation(ABaseCharacter *Character) override;
	
	
};
