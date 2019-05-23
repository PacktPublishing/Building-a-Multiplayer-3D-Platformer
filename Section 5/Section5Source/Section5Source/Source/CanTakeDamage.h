// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CanTakeDamage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanTakeDamage : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class MASTERINGUNREAL_API ICanTakeDamage
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage Interface")
		void ReceiveDamage(int amount);
};
