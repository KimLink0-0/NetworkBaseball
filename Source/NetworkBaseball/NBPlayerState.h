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
	void SetTurnCount(const uint8& NewTurnCount);
	FString GetPlayerInputValue() const { return PlayerInputValue; }
	void SetPlayerInputValue(const FString& NewInputValue) { PlayerInputValue = NewInputValue; }
	uint8 GetWinScore() const { return WinScore; }
	void SetWinScore(const uint8& NewWinScore) { WinScore = NewWinScore; }
	uint8 GetGameCount() const { return GameCount; }
	void SetGameCount(const uint8& NewGameCount);
	FString GetComputerGenNumber() const { return ComputerGenNumber; }
	void SetComputerGenNumber(const FString& NewGenNumber) { ComputerGenNumber = NewGenNumber; }

	void RequestNextGame();
	void UpdateScoreWidget();
	
	// OnRep 함수들
	UFUNCTION()
	void OnRep_UserName();
	UFUNCTION()
	void OnRep_WinScore();
	UFUNCTION()
	void OnRep_GameCount();
	UFUNCTION()
	void OnRep_TurnCount();
	UFUNCTION()
	void OnRep_GenNumber(); // 복제 후 UI 갱신 등 처리
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FName UserName;
	UPROPERTY(ReplicatedUsing = OnRep_TurnCount)
	uint8 TurnCount;
	UPROPERTY(ReplicatedUsing = OnRep_GenNumber)
	FString ComputerGenNumber;
	UPROPERTY()
	FString PlayerInputValue;
	UPROPERTY(ReplicatedUsing = OnRep_WinScore)
	uint8 WinScore;
	UPROPERTY(ReplicatedUsing = OnRep_GameCount)
	uint8 GameCount;
};
