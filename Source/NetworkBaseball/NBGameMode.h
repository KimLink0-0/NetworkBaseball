// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NBGameMode.generated.h"

class ANBPlayerState;

UCLASS()
class NETWORKBASEBALL_API ANBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANBGameMode();

	// Getter & Setter
	ANBPlayerState* GetPlayerStates(const FName& UserName) const { return PlayerStates.FindRef(UserName); }
	
	// Input
	void ReceivedInputMessage(const FName& UserName, const FText& NewInput) const;
	bool IsChatInput(const FText& NewInput) const;
	UFUNCTION()
	void SendChatToState(const FName& UserName, const FText& NewInput) const;
	UFUNCTION()
	void SendProgressToState(const FName& UserName, const FText& NewInput) const;

	// Game Rule
	void GenerateComputerNumber() const;
	FString JudgePlayResult(const FName& UserName, const FText& NewInput) const;
	void JudgePlay(const FName& UserName, const FText& NewInput) const;
	void BroadcastPlayResult(const FString& Result) const;
	void StartTurn(const FName& UserName) const;
	void NextGame(const FName& UserName) const;

	
	// Add Player
	void AssignDefaultUserName(const APlayerController* NewPlayer) const;
	void AddPlayerStatesToMap(const APlayerController* NewPlayer);


protected:
	TMap<FName, ANBPlayerState*> PlayerStates;
	uint8 MaxTurnCount = 3;
	uint8 MaxGameCount = 3;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

};
