// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "BaseCharacter.h"
#include "PlatformerGameState.generated.h"

/* ENUM FOR DEFINING MATCH STATE */
UENUM(BlueprintType)
enum class EMatchState : uint8 {
	ENotConnected				UMETA(DisplayName = "Not Connected"),
	EWaitingForPlayers			UMETA(DisplayName = "Waiting For Players"),
	EStartingGame				UMETA(DisplayName = "Starting Game"),
	EGameInProgress				UMETA(DisplayName = "Game In Progress"),
	EGameComplete				UMETA(DisplayName = "Game Complete"),
	ERestartingGame				UMETA(DisplayName = "Restarting Game"),

};

/* STRUCT FOR DEFINING A CHARACTER FOR THE CHARACTER SELECT SCREEN */
USTRUCT(BlueprintType)
struct FCharacterSpecification {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class ABaseCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		UTexture2D *CharacterIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		FString CharacterName;
};
/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APlatformerGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	APlatformerGameState();

	/* CLASS REFERENCES TO SCREENS TO BE DISPLAYED DURING A MATCH */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class UUserWidget> cWaitingForPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class UUserWidget> cGameInProgress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class UUserWidget> cChangingLevel;

	//Character list we'll specify in the editor:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TArray<FCharacterSpecification> Characters;

	//For changing state
	UFUNCTION(NetMulticast, BlueprintCallable, Reliable, Category = "Match State")
		void ChangeState(EMatchState newState);

	//get current state
	UFUNCTION(BlueprintCallable, Category = "Match State")
		EMatchState GetMatchState();

	//Tick function
	virtual void Tick(float DeltaTime) override;
	

	//Update match time
	UFUNCTION(Server, Reliable, WithValidation, Category = "Match State")
		void UpdateMatchTime(float NewTime);
private:
	//the currently displayed widget
	UUserWidget *CurrentWidget;

	//the current state, replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		EMatchState CurrentMatchState;

	UPROPERTY(Replicated, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		float TotalMatchTime;

	//for changing state
	void EnterState(EMatchState newState);
	void LeaveState();
};
