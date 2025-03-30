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
	ANBPlayerState* GetPlayerStates(const FName& PlayerName) const { return PlayerStates.FindRef(PlayerName); }
	uint8 GetMaxInning() const { return MaxInning; }
	uint8 GetMaxOutCount() const { return MaxOutCount; }
	uint8 GetMaxStrikeCount() const { return MaxStrikeCount; }
	uint8 GetMaxBallCount() const { return MaxBallCount; }

	//
	FString GenerateComputerNumber() const;
	FString JudgePlayResult(const FName& PlayerName);
	FString EndGame();

	void AddInningCount(const FName& PlayerName);
	void AddOutCount(const FName& PlayerName);
	void AddStrikeCount(const FName& PlayerName);
	void AddBallCount(const FName& PlayerName);

	void ResetBallCount(const FName& PlayerName);
	void ResetStrikeCount(const FName& PlayerName);
	void ResetOutCount(const FName& PlayerName);

	bool CheckCanContinueInning(const FName& PlayerName) const;

protected:
	TMap<FName, ANBPlayerState*> PlayerStates;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	uint8 MaxInning = 9;
	UPROPERTY()
	uint8 MaxOutCount = 3;
	UPROPERTY()
	uint8 MaxStrikeCount = 3;
	UPROPERTY()
	uint8 MaxBallCount = 4;
};


