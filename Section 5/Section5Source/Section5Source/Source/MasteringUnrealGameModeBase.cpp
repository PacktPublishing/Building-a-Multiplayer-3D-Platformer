// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "PlatformerGameState.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerState.h"
#include "MasteringUnrealGameModeBase.h"

AMasteringUnrealGameModeBase::AMasteringUnrealGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	TimeSinceStart = 0.f;
}

void AMasteringUnrealGameModeBase::Logout(AController *exiting)
{
	Super::Logout(exiting);

	//get game state
	APlatformerGameState *PlatGameState = Cast<APlatformerGameState>(GameState);

	if (PlatGameState) {
		//remove the player state from the list of players
		PlatGameState->RemovePlayerState(exiting->PlayerState);
	}
}

void AMasteringUnrealGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ensure we're on server
	if (Role == ROLE_Authority) {
		//get current game state
		APlatformerGameState *PlatGameState = Cast<APlatformerGameState>(GameState);

		//get game instance
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetGameInstance());

		if (GameInstance && PlatGameState) {
			//check we're in game
			if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) {
				//check match state
				switch (PlatGameState->GetMatchState()) {
				case EMatchState::ENotConnected: {
					PlatGameState->ChangeState(EMatchState::EWaitingForPlayers);
					break;
				}
				case EMatchState::EGameInProgress: {
					TimeSinceStart += DeltaTime;

					//check for player deaths and respawn them as appropriate
					for (auto &player : PlatGameState->PlayerArray) {
						//cast the state to PlatformerPlayerState
						APlatformerPlayerState *PlatPlayer = Cast<APlatformerPlayerState>(player);

						//if the player is valid and is not a spectator
						if (PlatPlayer && !PlatPlayer->bSpectator) {
							//check if the player is dead
							if (PlatPlayer->GetCurrentHealth() <= 0) {
								//get the player controller
								APlayerController *PlayerController = PlatPlayer->GetNetOwningPlayer()->PlayerController;
								//triger stat reset
								PlatPlayer->PlayerRespawnedAfterDeath();

								//respawn the player
								PlayerController->GetPawn()->Destroy();
								PlayerController->ServerRestartPlayer();
							}
						}
					}

					PlatGameState->UpdateMatchTime(TimeSinceStart);

					break;
				}
				case EMatchState::EGameComplete:{
						break;
					}
				default: {
					//any other match state will reset timesince start to 0
					if (TimeSinceStart != 0.f) {
						TimeSinceStart = 0.f;
						PlatGameState->UpdateMatchTime(TimeSinceStart);
					}
				}
				}
			}
		}
	}
}

UClass * AMasteringUnrealGameModeBase::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	//Get game state and game instance
	APlatformerGameState *PlatGameState = Cast<APlatformerGameState>(GameState);
	UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetGameInstance());

	if (PlatGameState && GameInstance) {
		//check whether we're in game
		if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) {
			//we check whether the game is in progress or is starting
			if (PlatGameState->GetMatchState() == EMatchState::EGameInProgress ||
				PlatGameState->GetMatchState() == EMatchState::EStartingGame) {

				//get the player state
				APlatformerPlayerState *PlayerState = Cast<APlatformerPlayerState>(InController->PlayerState);

				if (PlayerState) {
					//check if we're a spectator or that the player is not ready, meaning they joined after the game
					//had started
					if (PlayerState->bSpectator || !PlayerState->bIsReady) {
						//return the spectator class
						return ASpectatorPawn::StaticClass();
					}

					//otherwise we return the selected character
					int SelectedCharacter = PlayerState->SelectedCharacterIndex;

					//if it is a valid index, return that class
					if (PlatGameState->Characters.IsValidIndex(SelectedCharacter)) {
						return PlatGameState->Characters[SelectedCharacter].CharacterClass;
					}
					else { //if the index is invalid, return the first character in the list
						return PlatGameState->Characters[0].CharacterClass;

					}
				}
			}
		}
	}

	//If the match isnt starting and isnt in progress, we default to spectator
	return ASpectatorPawn::StaticClass();
}
