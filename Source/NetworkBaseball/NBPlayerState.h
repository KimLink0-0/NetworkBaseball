// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBPlayerState.generated.h"

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
	void SetTurnCount(const uint8& NewTurnCount) { TurnCount = NewTurnCount; }
	FString GetPlayerInputValue() const { return PlayerInputValue; }
	void SetPlayerInputValue(const FString& NewInputValue) { PlayerInputValue = NewInputValue; }
	uint8 GetWinScore() const { return WinScore; }
	void SetWinScore(const uint8& NewWinScore) { WinScore = NewWinScore; }
	uint8 GetGameCount() const { return GameCount; }
	void SetGameCount(const uint8& NewGameCount);
	
	void ResetTurnCount() { TurnCount = 0; }

	void RequestNextGame();
	
	UFUNCTION()
	void OnRep_WinScore();
	UFUNCTION()
	void OnRep_GameCount();
	UFUNCTION()
	void OnRep_TurnCount();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FName UserName;
	UPROPERTY(ReplicatedUsing = OnRep_TurnCount)
	uint8 TurnCount;
	UPROPERTY()
	FString PlayerInputValue;
	UPROPERTY(ReplicatedUsing = OnRep_WinScore)
	uint8 WinScore;
	UPROPERTY(ReplicatedUsing = OnRep_GameCount)
	uint8 GameCount;

	UFUNCTION()
	void OnRep_UserName();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};