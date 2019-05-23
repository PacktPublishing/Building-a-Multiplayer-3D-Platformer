// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "PlatformerGameInstance.generated.h"

/**
 * 
 */

/* ENUM FOR CONTROLLING GAME STATE */
UENUM(BlueprintType)
enum class EGameState : uint8 {
	ENone				UMETA(DisplayName = "None"),
	ELoadingScreen		UMETA(DisplayName = "Loading"),
	EStartup			UMETA(DisplayName = "Startup"),
	EMainMenu			UMETA(DisplayName = "Main Menu"),
	EMultiplayerHome	UMETA(DisplayName = "Multiplayer Home"),
	EMultiplayerJoin	UMETA(DisplayName = "Mulitplayer Join"),
	EMultiplayerHost	UMETA(DisplayName = "Multiplayer Host"),
	EMultiplayerInGame	UMETA(DisplayName = "Multiplayer In Game"),
	ETravelling			UMETA(DisplayName = "Travelling"),
};

/* ENUM TO TRACK INPUT STATES */
UENUM(BlueprintType)
enum class EInputMode : uint8 {
	EUIOnly				UMETA(DisplayName = "UI Only"),
	EUIAndGame			UMETA(DisplayName = "UI And Game"),
	EGameOnly			UMETA(DisplayName = "Game Only"),
};

UCLASS()
class MASTERINGUNREAL_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//Constructor
	UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer);

	/* Widget references */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Manager")
		TSubclassOf<class UUserWidget> cMainMenu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Manager")
		TSubclassOf<class UUserWidget> cMPHome;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Manager")
		TSubclassOf<class UUserWidget> cMPJoin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Manager")
		TSubclassOf<class UUserWidget> cMPHost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "State Manager")
		TSubclassOf<class UUserWidget> cLoadingScreen;

	/* STATE CHANGES */
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void ChangeState(EGameState newState);
	
	//Get current game state
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		EGameState GetCurrentGameState();

	/* Input mode handling */
	//function for setting the input mode
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void SetInputMode(EInputMode newInputMode, bool bShowMouseCursor);

	//Current Input Mode
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Game Instance")
		EInputMode CurrentInputMode;
	//are we displaying the mouse cursor
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Game Instance")
		bool bIsShowingMouseCursor;
	

private:
	//currently displayed widget
	UUserWidget *currentWidget;
	//our current game state
	EGameState currentState;

	//function for entering a state
	void EnterState(EGameState newState);
	//function for leaving a state
	void LeaveState();
};
