// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "BaseCharacter.h"
#include "Powerup.h"

void APowerup::CollectPickup_Implementation(ABaseCharacter * Character)
{
	if (Role != ROLE_Authority) return;

	if (Character) {
		//add powerup to inventory
		Character->CollectPowerup(this);
		bIsPickedUp = true;

		//Destroy the mes and collision box as they no longer exist in world
		CollisionSphere->DestroyComponent();
		Mesh->DestroyComponent();
	}
}

void APowerup::UsePowerup(ABaseCharacter * Character, FVector LaunchLocation, FVector LaunchDirection)
{
	//we're going to leave this empty as the base class won't be doing anything.
}
