// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "MasteringUnrealGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API AMasteringUnrealGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMasteringUnrealGameModeBase();

	virtual void Logout(AController *exiting) override;
	virtual void Tick(float DeltaTime) override;

	//for spawning appropriate character
	virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;
	
private:

	float TimeSinceStart;
};
