// Fill out your copyright notice in the Description page of Project Settings.

#include "MasteringUnreal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "PlatformerGameInstance.h"



UPlatformerGameInstance::UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer) 
{
	//initial state is None
	currentState = EGameState::ENone;
	//current widget is nothing
	currentWidget = nullptr;
}

void UPlatformerGameInstance::ChangeState(EGameState newState)
{
	//change state if we're not already in this state
	if (newState != currentState) {
		LeaveState();
		EnterState(newState);
	}
}

EGameState UPlatformerGameInstance::GetCurrentGameState()
{
	return currentState;
}

void UPlatformerGameInstance::SetInputMode(EInputMode newInputMode, bool bShowMouseCursor)
{
	// Set input mode according to NewInputMode
	switch (newInputMode) {
	case EInputMode::EUIOnly: {
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		break;
	}
	case EInputMode::EUIAndGame: {
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		break;
	}
	case EInputMode::EGameOnly: {
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		break;
	}
	}

	//show or hide the mouse cursor
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bShowMouseCursor;

	//retain the values for further use
	CurrentInputMode = newInputMode;
	bIsShowingMouseCursor = bIsShowingMouseCursor;
}

void UPlatformerGameInstance::EnterState(EGameState newState)
{
	//set the current state to newState
	currentState = newState;

	switch (currentState) {
	case EGameState::ELoadingScreen: {
		//create the widget
		currentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cLoadingScreen);
		//add the widget to the viewport
		currentWidget->AddToViewport();

		//go to the appropriate input mode
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMainMenu: {
		//create the widget
		currentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMainMenu);
		//add the widget to the viewport
		currentWidget->AddToViewport();

		//go to the appropriate input mode
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerHome: {
		//create the widget
		currentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHome);
		//add the widget to the viewport
		currentWidget->AddToViewport();

		//go to the appropriate input mode
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}

	case EGameState::EMultiplayerJoin: {
		//create the widget
		currentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPJoin);
		//add the widget to the viewport
		currentWidget->AddToViewport();

		//go to the appropriate input mode
		SetInputMode(EInputMode::EUIOnly, true);
		break;
	}
	case EGameState::EMultiplayerHost: {
		//create the widget
		currentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHost);
		//add the widget to the viewport
		currentWidget->AddToViewport();

		//go to the appropriate input mode
		SetInputMode(EInputMode::EUIOnly, true);

		break;
	}
	case EGameState::EMultiplayerInGame: {
		SetInputMode(EInputMode::EGameOnly, false);
		break;
	}
	case EGameState::ETravelling: {
		SetInputMode(EInputMode::EUIOnly, false);
		break;
	}
	case EGameState::ENone: {
		break;
	}
	default: {
		break;
	}
	}
}

void UPlatformerGameInstance::LeaveState()
{

	switch (currentState) {
	case EGameState::ELoadingScreen: {
		
	}
	case EGameState::EMainMenu: {
		
	}
	case EGameState::EMultiplayerHome: {
		
	}
	case EGameState::EMultiplayerJoin: {
		
	}
	case EGameState::EMultiplayerHost: {
		if (currentWidget) {
			currentWidget->RemoveFromViewport();
			currentWidget = nullptr;
		}
		break;
	}
	case EGameState::EMultiplayerInGame: {
		break;
	}
	case EGameState::ETravelling: {
		break;
	}
	case EGameState::ENone: {
		break;
	}
	default: {
		break;
	}
	}

	EnterState(EGameState::ENone);
}
