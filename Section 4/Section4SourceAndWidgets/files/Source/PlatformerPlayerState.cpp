// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "UnrealNetwork.h"
#include "PlatformerPlayerState.h"

APlatformerPlayerState::APlatformerPlayerState()
{
	NumberOfCoinsCollected = 0;
	MaxHealth = 3;
	CurrentHealth = MaxHealth;
	NumberOfDeaths = 0;
	SelectedPowerup = nullptr;
	bIsReady = false;
	bSpectator = false;
	SelectedCharacterIndex = 0;
}

void APlatformerPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlatformerPlayerState, CurrentHealth);
	DOREPLIFETIME(APlatformerPlayerState, NumberOfCoinsCollected);
	DOREPLIFETIME(APlatformerPlayerState, NumberOfDeaths);
	DOREPLIFETIME(APlatformerPlayerState, SelectedPowerup);
	DOREPLIFETIME(APlatformerPlayerState, bIsReady);
	DOREPLIFETIME(APlatformerPlayerState, bSpectator);
	DOREPLIFETIME(APlatformerPlayerState, SelectedCharacterIndex);
}

void APlatformerPlayerState::CollectCoin()
{
	NumberOfCoinsCollected += 1;
}

void APlatformerPlayerState::CollectHeart()
{
	CurrentHealth += 1;

	//clamp between 0 and MaxHealth
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void APlatformerPlayerState::ReceiveDamage(int amount)
{
	CurrentHealth -= amount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void APlatformerPlayerState::ToggleReady_Implementation()
{
	bIsReady = !bIsReady;
}

bool APlatformerPlayerState::ToggleReady_Validate() {
	return true;
}

void APlatformerPlayerState::ToggleSpectator_Implementation()
{
	bSpectator = !bSpectator;
}

bool APlatformerPlayerState::ToggleSpectator_Validate() {
	return true;
}

void APlatformerPlayerState::SelectCharacter_Implementation(int index)
{
	SelectedCharacterIndex = index;
}

bool APlatformerPlayerState::SelectCharacter_Validate(int index) {
	return true;
}
