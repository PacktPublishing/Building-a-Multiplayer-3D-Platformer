// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "BaseCharacter.h"
#include "Pickup_Coin.h"

APickup_Coin::APickup_Coin()
{

}

void APickup_Coin::CollectPickup_Implementation(ABaseCharacter * Character)
{
	if (Role != ROLE_Authority) return;

	if (Character) {
		//Collect the coin
		Character->CollectCoin();
		bIsPickedUp = true;
		this->Destroy();
	}
}
