// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//Constructor
	APlatformerPlayerController();

	//To hold a reference to our UMG HUD widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Player Controller")
		TSubclassOf<class UUserWidget> cHUD;

	//HUD instance
	UUserWidget *HUD;

	//to hide or show pause menu
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
		bool bPauseMenuDisplayed;

	EInputMode modeBeforePause;
	bool bShowCursorBeforePause;

	//to hide or show the player list
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
		bool bPlayerListDisplayed;


	//To setup the input component
	virtual void SetupInputComponent() override;

	//tick
	virtual void Tick(float DeltaTime) override;

	//endplay
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	void TogglePauseMenu();

	void ShowPlayerList();
	void HidePlayerList();

	UFUNCTION(BlueprintCallable, Category = "Platformer Player Controller")
		void HideAllMenus();
};
