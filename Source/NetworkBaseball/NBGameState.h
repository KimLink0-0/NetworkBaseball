// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NBGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKBASEBALL_API ANBGameState : public AGameState
{
	GENERATED_BODY()

public:
	ANBGameState();
	
	// Getter & Setter
	uint8 GetStrikeCount() const { return StrikeCount; }
	void SetStrikeCount(const uint8& NewValue) { StrikeCount = NewValue; }
	uint8 GetBallCount() const { return BallCount; }
	void SetBallCount(const uint8& NewValue) { BallCount = NewValue; }
	uint8 GetOutCount() const { return OutCount; }
	void SetOutCount(const uint8& NewValue) { OutCount = NewValue; }
	uint8 GetTurnCount() const { return TurnCount; }
	void SetTurnCount(const uint8& NewValue) { TurnCount = NewValue; }
	FString GetComputerNumber() const { return CurrentTurnComputerNumber; }
	void SetComputerNumber(const FString& NewValue) { CurrentTurnComputerNumber = NewValue; }

	// Reset
	void ResetStrikeCount() { StrikeCount = 0; }
	void ResetBallCount() { BallCount = 0; }
	void ResetOutCount() { OutCount = 0; }

	// Replication
	UFUNCTION()
	void OnRep_StrikeCount();
	UFUNCTION()
	void OnRep_BallCount();
	UFUNCTION()
	void OnRep_OutCount();
	UFUNCTION()
	void OnRep_TurnCount();
	UFUNCTION()
	void OnRep_ComputerNumber();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_StrikeCount)
	uint8 StrikeCount;
	UPROPERTY(ReplicatedUsing = OnRep_BallCount)
	uint8 BallCount;
	UPROPERTY(ReplicatedUsing = OnRep_OutCount)
	uint8 OutCount;
	UPROPERTY(ReplicatedUsing = OnRep_TurnCount)
	uint8 TurnCount;
	UPROPERTY(ReplicatedUsing = OnRep_ComputerNumber)
	FString CurrentTurnComputerNumber;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
