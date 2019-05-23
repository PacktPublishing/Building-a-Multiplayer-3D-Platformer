// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "PlatformerGameInstance.generated.h"

/* ENUM FOR TRACKING GAME STATE */
UENUM(BlueprintType)
enum class EGameState : uint8 {
	ENone				UMETA(DisplayName = "None"),
	ELoadingScreen		UMETA(DisplayName = "Loading Screen"),
	EStartup			UMETA(DisplayName = "Startup"),
	EMainMenu			UMETA(DisplayName = "Main Menu"),
	EMultiplayerHome	UMETA(DisplayName = "Multiplayer Home"),
	EMultiplayerJoin	UMETA(DisplayName = "Multiplayer Join"),
	EMultiplayerHost	UMETA(DisplayName = "Multiplayer Host"),
	EMultiplayerInGame	UMETA(DisplayName = "Multiplayer In Game"),
	ETravelling			UMETA(DisplayName = "Travelling"),
};

/* ENUM TO TRACK INPUT STATES */
UENUM(BlueprintType)
enum class EInputMode : uint8 {
	EUIOnly			UMETA(DisplayName = "UI Only"),
	EUIAndGame		UMETA(DisplayName = "UI And Game"),
	EGameOnly		UMETA(DisplayName = "Game Only"),
};

/* Type for providing special settings for the hosted server */
USTRUCT(BlueprintType)
struct FBlueprintSessionSetting {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString key;

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString value;
};

/* Type for holding our SearchResult and exposing the needed properties to blueprints*/
USTRUCT(BlueprintType)
struct FBlueprintSearchResult {
	GENERATED_BODY()

	//Our search result. this type is not blueprint accessible
	FOnlineSessionSearchResult result;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bIsInProgress;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString MapName;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int PingInMs;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int CurrentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int MaxPlayers;

	//Constructor for empty Search Result
	FBlueprintSearchResult() {
		ServerName = FString("No Server Info");
		PingInMs = -1;
		MapName = FString("No Map Info");
		CurrentPlayers = 0;
		MaxPlayers = 0;
	}

	//Constructor when provided a search result
	FBlueprintSearchResult(FOnlineSessionSearchResult theResult) 
	{
		//keep the result
		result = theResult;

		//retrieve special settings from the search result
		ServerName = GetSpecialSettingString(FString("ServerName"));
		MapName = GetSpecialSettingString(FString("MAPNAME"));
		FString InProgressString = GetSpecialSettingString(FString("InProgress"));

		if (InProgressString == FString("true")) {
			bIsInProgress = true;
		}
		else {
			bIsInProgress = false;
		}

		//get some built in setting data
		MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
		CurrentPlayers = MaxPlayers - result.Session.NumOpenPublicConnections;
		PingInMs = result.PingInMs;
	}

	//function for getting special setting data from our result
	FString GetSpecialSettingString(FString key) {
		//retrieve the session settings
		FOnlineSessionSettings settings = result.Session.SessionSettings;

		//check to see if the key exists
		if (settings.Settings.Contains(FName(*key))) {
			FString value;

			//retrieve the value from the settings
			settings.Settings[FName(*key)].Data.GetValue(value);
			return value;
		}

		//if it doesnt contain that setting
		return FString("NO DATA AT THAT KEY");
	}
};
/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//constructor
	UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer);

	//Init function
	virtual void Init() override;

	/* Widget references, these will be what is displayed on screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMainMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPHome;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPJoin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPHost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cLoadingScreen;

	/* STATE CHANGES */
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void ChangeState(EGameState newState);

	//get the current game state
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		EGameState GetGameState();

	/* INPUT MODE HANDLING */
	//function for setting the input mode
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void SetInputMode(EInputMode newInputMode, bool bShowMouseCursor);

	//current input mode
	EInputMode CurrentInputMode;
	//are we currently displaying the mouse cursor
	bool bIsShowingMouseCursor;

	/* Online Session Interface */

	//Shared pointer for holding session settings
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/* SESSION CREATION */
	//function for hosting a session from blueprints
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings);

	//c++ function for hosting a session
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap);

	//delegate function which will be called when session is created
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//delegate to be called when session is created
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	//delegate handle retained after registering delegate
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/* STARTING A SESSION */

	//delegate function which will be called when session is started
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	//delegate to be called when session is started
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	//delegate handle retained after registering delegate
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/* FINDING SESSIONS */

	//Array for holding our search results
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		TArray<FBlueprintSearchResult> searchResults;

	//booleans for keeping track of search status
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bHasFinishedSearchingForGames;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bSearchingForGames;

	//shared pointer to our c++ native search results
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//blueprint function for finding games
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void FindGames(bool bIsLAN);

	//c++ function for finding sessions
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN);

	//delegate function called when FindSessions completes
	void OnFindSessionsComplete(bool bWasSuccessful);

	//Delegate for OnFindSessionsComplete
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	//delegate handle for OnFindSessionsComplete
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;


	/* JOIN SESSIONS */
	//Blueprint function for joining a session
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void JoinGame(FBlueprintSearchResult result);

	//c++ function for joining the session
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//delegate function which will be called when a session has been joined
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//delegate for binding our function
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	//delegate handle for OnJoinSessionCompleteDelegate
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/* UPDATING SESSION */
	//function for getting the current value of a special setting for the active session
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		FString GetSessionSpecialSettingString(FString key);

	//function for creating a new or updating an existing special setting on the active session
	//host only
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting);

	//delegate function which will be called after update session completes
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);

	//delegate which will be called after updating a session
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

	//delegate handle for OnUpdateSessionCompleteDelegate
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	/* DESTROYING A SESSION / LEAVING GAME */
	//Blueprint function for leaving game
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void LeaveGame();

	//delegate function which will be called after leaving session
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//delegate which will be called after destroying a session
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	//delegate handle for OnDestroySessionCompleteDelegate
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/* HANDLE NETWORK ERRORS */
	void HandleNetworkError(UWorld *World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);
private:

	//currently displayed widget
	UUserWidget *CurrentWidget;

	//current game state
	EGameState CurrentState;

	//function for entering a state
	void EnterState(EGameState newState);
	//function for leaving current state
	void LeaveState();
	
	
};
