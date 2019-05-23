// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "Fireball.h"
#include "Powerup_Fireball.h"


APowerup_Fireball::APowerup_Fireball()
{

}

void APowerup_Fireball::UsePowerup(ABaseCharacter * Character, FVector LaunchLocation, FVector LaunchDirection)
{
	AFireball *newFireball = GetWorld()->SpawnActor<AFireball>(LaunchLocation, FRotator::ZeroRotator);
	newFireball->TheOwner = (ACharacter*)Character;
	newFireball->CollisionSphere->AddForce(LaunchDirection * 5000);
}
