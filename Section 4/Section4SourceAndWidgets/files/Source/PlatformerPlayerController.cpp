// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "PlatformerPlayerController.h"

APlatformerPlayerController::APlatformerPlayerController() {

	//enable tick function
	PrimaryActorTick.bCanEverTick = true;

	bPauseMenuDisplayed = false;
	bPlayerListDisplayed = false;

}

void APlatformerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this, &APlatformerPlayerController::TogglePauseMenu);
	InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Pressed, this, &APlatformerPlayerController::ShowPlayerList);
	InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Released, this, &APlatformerPlayerController::HidePlayerList);
}

void APlatformerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//check to ensure this is the locally owned controller
	if (IsLocalController()) {
		//get the current game instance
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		//check for success
		if (GameInstance) {
			//if we're currently in game, create the HUD
			if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) {
				//check if HUD already exists
				if (HUD == nullptr) {
					//if not, create the HUD
					if (cHUD) {
						HUD = CreateWidget<UUserWidget>(this, cHUD);
						HUD->AddToViewport();
					}
				}
			}
			else if (HUD != nullptr) { //if we're not in game and a HUD exists, destroy it
				HUD->RemoveFromViewport();
				HUD = nullptr;
			}
		}
	}
}

void APlatformerPlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HUD) {
		HUD->RemoveFromViewport();
		HUD = nullptr;
	}
}

void APlatformerPlayerController::TogglePauseMenu()
{
	bPauseMenuDisplayed = !bPauseMenuDisplayed;
		
	//get the current game instance
	UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance) {
		if (bPauseMenuDisplayed) {

			if (bPlayerListDisplayed) {
				bPlayerListDisplayed = false;
			}
			//retain the old input mode
			modeBeforePause = GameInstance->CurrentInputMode;
			bShowCursorBeforePause = GameInstance->bIsShowingMouseCursor;

			//set input mode to UI and Game
			GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
		}
		else {
			//restore old input mode
			GameInstance->SetInputMode(modeBeforePause, bShowCursorBeforePause);
		}
	}
}

void APlatformerPlayerController::ShowPlayerList()
{
	//we dont want to allow the player screen to be displayed if the pause menu is displayed
	if (!bPauseMenuDisplayed) {
		bPlayerListDisplayed = true;

		//Set input to UI and Game to allow scrolling through players on a populated server
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
		}
	}
}

void APlatformerPlayerController::HidePlayerList()
{
	if (bPlayerListDisplayed) {
		bPlayerListDisplayed = false;

		//set it back to game only and hide cursor
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) {
			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}
	}
}

void APlatformerPlayerController::HideAllMenus()
{
	HidePlayerList();

	if (bPauseMenuDisplayed) {
		TogglePauseMenu();
	}
}
