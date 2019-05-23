// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Powerup.h"
#include "PlatformerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APlatformerPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public: 
	APlatformerPlayerState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Player State")
		APowerup *SelectedPowerup;

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void CollectCoin();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void CollectHeart();

	UFUNCTION(BlueprintCallable, Category = "Player State")
		void ReceiveDamage(int amount);
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int NumberOfCoinsCollected;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int CurrentHealth;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int NumberOfDeaths;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Player State")
		int MaxHealth;

	
	
};
