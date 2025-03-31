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
	uint8 GetMaxInning() const { return MaxInning; }
	uint8 GetMaxOutCount() const { return MaxOutCount; }
	uint8 GetMaxStrikeCount() const { return MaxStrikeCount; }
	uint8 GetMaxBallCount() const { return MaxBallCount; }

	//
	bool CheckCanContinueInning(const FName& UserName) const;
	FString GenerateComputerNumber() const;
	FString JudgePlayResult(const FName& UserName);

	bool AddInningCount(const FName& UserName);
	bool AddOutCount(const FName& UserName);
	bool AddStrikeCount(const FName& UserName);
	bool AddBallCount(const FName& UserName);
	void AddHitCount(const FName& UserName);
	void AddGameScore(const FName& UserName);

	void ResetBallCount(const FName& UserName);
	void ResetStrikeCount(const FName& UserName);
	void ResetOutCount(const FName& UserName);
	void ResetHitCount(const FName& UserName);

	void AssignDefaultUserName(const APlayerController* NewPlayer);
	void AddPlayerStatesToMap(const APlayerController* NewPlayer);
	void InitNBGame(const APlayerController* NewPlayer);
	void EndNBGame(const FName& UserName);

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



