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
	
	uint8 GetInningCount() const { return InningCount; }
	void SetInningCount(const uint8& NewCount) { InningCount = NewCount; }
	uint8 GetOutCount() const { return OutCount; }
	void SetOutCount(const uint8& NewCount) { OutCount = NewCount; }
	uint8 GetStrikeCount() const { return StrikeCount; }
	void SetStrikeCount(const uint8& NewCount) { StrikeCount = NewCount; }
	uint8 GetBallCount() const { return BallCount; }
	void SetBallCount(const uint8& NewCount) { BallCount = NewCount; }
	
	FString GetUserPitchString() const { return UserPitchString; }
	void SetUserPitchString(const FString& NewNumberString) { UserPitchString = NewNumberString; }
	FString GetComputerPitchString() const { return ComputerPitchString; }
	void SetComputerPitchString(const FString& NewNumberString) { ComputerPitchString = NewNumberString; }

	//
	void RequestGenerateComputerPitch();
	
protected:
	UPROPERTY()
	FName UserName;
	
	UPROPERTY()
	uint8 InningCount;
	UPROPERTY()
	uint8 OutCount;
	UPROPERTY()
	uint8 StrikeCount;
	UPROPERTY()
	uint8 BallCount;

	UPROPERTY()
	FString UserPitchString;
	UPROPERTY()
	FString ComputerPitchString;
	
};