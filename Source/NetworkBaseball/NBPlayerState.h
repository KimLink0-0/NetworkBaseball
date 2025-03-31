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

	FName GetUserName() const { return UserName; }
	void SetUserName(const FName& NewName) { UserName = NewName; }
	
	uint8 GetInningCount() const { return InningCount; }
	void SetInningCount(const uint8& NewInningCount) { InningCount = NewInningCount; }
	uint8 GetOutCount() const { return OutCount; }
	void SetOutCount(const uint8& NewOutCount) { OutCount = NewOutCount; }
	uint8 GetStrikeCount() const { return StrikeCount; }
	void SetStrikeCount(const uint8& NewStrikeCount) { StrikeCount = NewStrikeCount; }
	uint8 GetBallCount() const { return BallCount; }
	void SetBallCount(const uint8& NewBallCount) { BallCount = NewBallCount; }

	int32 GetHitCount() const { return HitCount; }
	void SetHitCount(const int32& NewHitCount) { HitCount = NewHitCount; }
	int32 GetGameScore() const { return GameScore; }
	void SetGameScore(const int32& NewGameScore) { GameScore = NewGameScore; }
	
	FString GetUserPitchString() const { return UserPitchString; }
	void SetUserPitchString(const FString& NewValue);
	FString GetComputerPitchString() const { return ComputerPitchString; }
	void SetComputerPitchString(const FString& NewComputerPitchString) { ComputerPitchString = NewComputerPitchString; }
	FString GetPlayResult() const { return PlayResult; }
	void SetPlayResult(const FString& NewPlayResult) { PlayResult = NewPlayResult; }

	void ResetUserPitchString();
	void RequestGenerateComputerPitch();
	void RequestUpdateScreen();
	void RequestUpdateProgress();
	void RequestJudgePlayResult();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_UserName)
	FName UserName;
	
	UPROPERTY(ReplicatedUsing = OnRep_InningCount)
	uint8 InningCount;
	UPROPERTY(ReplicatedUsing = OnRep_OutCount)
	uint8 OutCount;
	UPROPERTY(ReplicatedUsing = OnRep_StrikeCount)
	uint8 StrikeCount;
	UPROPERTY(ReplicatedUsing = OnRep_BallCount)
	uint8 BallCount;

	UPROPERTY(ReplicatedUsing = OnRep_HitCount)
	int32 HitCount;
	UPROPERTY(ReplicatedUsing = OnRep_GameScore)
	int32 GameScore;

	UPROPERTY(ReplicatedUsing = OnRep_UserPitchString)
	FString UserPitchString;
	UPROPERTY(ReplicatedUsing = OnRep_ComputerPitchString)
	FString ComputerPitchString;
	UPROPERTY(ReplicatedUsing = OnRep_JudgePlayResult)
	FString PlayResult;

	UFUNCTION()
	void OnRep_UserName();
	UFUNCTION()
	void OnRep_InningCount();
	UFUNCTION()
	void OnRep_OutCount();
	UFUNCTION()
	void OnRep_StrikeCount();
	UFUNCTION()
	void OnRep_BallCount();
	UFUNCTION()
	void OnRep_HitCount();
	UFUNCTION()
	void OnRep_GameScore();
	UFUNCTION()
	void OnRep_UserPitchString();
	UFUNCTION()
	void OnRep_ComputerPitchString();
	UFUNCTION()
	void OnRep_JudgePlayResult();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
