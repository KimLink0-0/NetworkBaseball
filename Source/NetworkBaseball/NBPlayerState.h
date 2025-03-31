// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKBASEBALL_API ANBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ANBPlayerState();
	
	// Getter && Setter
	FName GetUserName() const { return UserName; }
	void SetUserName(const FName& NewName) { UserName = NewName; }
	uint8 GetTurnCount() const { return TurnCount; }
	void SetTurnCount(uint8 NewTurnCount) { TurnCount = NewTurnCount; }
	FString GetPlayerInputValue() const { return PlayerInputValue; }
	void SetPlayerInputValue(const FString& NewInputValue) { PlayerInputValue = NewInputValue; }
	uint8 GetWinScore() const { return WinScore; }
	void SetWinScore(uint8 NewWinScore) { WinScore = NewWinScore; }
	
protected:
	UPROPERTY()
	FName UserName;

	UPROPERTY()
	uint8 TurnCount;
	
	UPROPERTY()
	FString PlayerInputValue;

	UPROPERTY()
	uint8 WinScore;
};