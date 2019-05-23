// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerState.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "PlatformerGameState.h"

APlatformerGameState::APlatformerGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentWidget = nullptr;
	CurrentMatchState = EMatchState::ENotConnected;
}

void APlatformerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlatformerGameState, CurrentMatchState);
	DOREPLIFETIME(APlatformerGameState, TotalMatchTime);
	DOREPLIFETIME(APlatformerGameState, Winner);
}

void APlatformerGameState::ChangeState_Implementation(EMatchState newState)
{
	if (CurrentMatchState != newState) {
		LeaveState();
		EnterState(newState);
	}
}

EMatchState APlatformerGameState::GetMatchState()
{
	return CurrentMatchState;
}

void APlatformerGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ensure we're not out of sync with server
	//get player state
	APlatformerPlayerState *PlayerState = Cast<APlatformerPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

	if (PlayerState) {
		//if we're out of sync
		if (PlayerState->CurrentLocalMatchState != CurrentMatchState) {
			LeaveState();
			EnterState(CurrentMatchState);
		}
	}
}

void APlatformerGameState::SetWinner_Implementation(APlayerState * NewWinner)
{
	if (Role == ROLE_Authority) {
		Winner = NewWinner;
	}
}

bool APlatformerGameState::SetWinner_Validate(APlayerState * NewWinner)
{
	return true;
}

void APlatformerGameState::UpdateMatchTime_Implementation(float NewTime)
{
	TotalMatchTime = NewTime;
}

bool APlatformerGameState::UpdateMatchTime_Validate(float NewTime)
{
	return true;
}

void APlatformerGameState::EnterState(EMatchState newState)
{
	CurrentMatchState = newState;

	//Change state on player
	APlatformerPlayerState *CurrentPlayerState = Cast<APlatformerPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

	if (CurrentPlayerState) {
		CurrentPlayerState->CurrentLocalMatchState = CurrentMatchState;
	}

	//switch on new state
	switch (CurrentMatchState) {
	case EMatchState::ENotConnected: {
		break;
	}
	case EMatchState::EWaitingForPlayers: {
		//Show waiting for players UI
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cWaitingForPlayers);
		CurrentWidget->AddToViewport();

		//change input mode
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		}

		if (Role == ROLE_Authority) {
			//set in progress session setting to true
			FBlueprintSessionSetting NewSetting;
			NewSetting.key = FString("InProgress");
			NewSetting.value = FString("false");

			GameInstance->SetOrUpdateSessionSpecialSettingString(NewSetting);
		}

		break;
	}
	case EMatchState::EStartingGame: {

		//Change to Game In Progress
		if (Role == ROLE_Authority) {
			//respawn all connected players if we're the server
			for (auto &player : PlayerArray) {
				//destroy the current pawn
				player->GetNetOwningPlayer()->PlayerController->GetPawn()->Destroy();
				//spawn the new pawn
				player->GetNetOwningPlayer()->PlayerController->ServerRestartPlayer();
			}

			ChangeState(EMatchState::EGameInProgress);
		}
		break;
	}
	case EMatchState::EGameInProgress: {

		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cGameInProgress);
			CurrentWidget->AddToViewport();

			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}

		if (Role == ROLE_Authority) {
			//set in progress session setting to true
			FBlueprintSessionSetting NewSetting;
			NewSetting.key = FString("InProgress");
			NewSetting.value = FString("true");

			GameInstance->SetOrUpdateSessionSpecialSettingString(NewSetting);
		}
		break;
	}
	case EMatchState::EGameComplete: {

		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (Role == ROLE_Authority) {
			//respawn all players as spectator
			for (auto &player : PlayerArray) {
				APlayerController *PC = player->GetNetOwningPlayer()->PlayerController;

				PC->GetPawn()->Destroy();
				PC->ServerRestartPlayer();
			}
		}

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cGameComplete);
		CurrentWidget->AddToViewport();

		GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EMatchState::ERestartingGame: {

		//get the game isntance
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			//display the change level widget
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cChangingLevel);
			CurrentWidget->AddToViewport();

			GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		}

		//respawn the players as spectators
		if (Role == ROLE_Authority) {

			Winner = nullptr;

			//respawn all connected players if we're the server
			for (auto &player : PlayerArray) {
				//destroy the current pawn
				player->GetNetOwningPlayer()->PlayerController->GetPawn()->Destroy();
				//spawn the new pawn
				player->GetNetOwningPlayer()->PlayerController->ServerRestartPlayer();
			}

			//Travel to a new instance of our map by getting the session setting
			//"MAPNAME" and passing it to ServerTravel
			FString mapname = GameInstance->GetSessionSpecialSettingString("MAPNAME");
			GetWorld()->ServerTravel(mapname);
		}
		break;
	}
	}
}

void APlatformerGameState::LeaveState()
{
	//switch on new state
	switch (CurrentMatchState) {
	case EMatchState::ENotConnected: {
//		break;
	}
	case EMatchState::EWaitingForPlayers: {
//		break;
	}
	case EMatchState::EStartingGame: {
//		break;
	}
	case EMatchState::EGameInProgress: {
//		break;
	}
	case EMatchState::EGameComplete: {
//		break;
	}
	case EMatchState::ERestartingGame: {
		//hide any widget
		if (CurrentWidget) {
			CurrentWidget->RemoveFromViewport();
			CurrentWidget = nullptr;
		}
		break;
	}
	}
}

