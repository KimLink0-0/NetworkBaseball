// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameMode.h"

#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "NetworkBaseball.h"


ANBGameMode::ANBGameMode()
{
	PlayerControllerClass = ANBPlayerController::StaticClass();
	PlayerStateClass = ANBPlayerState::StaticClass();
	
}

bool ANBGameMode::CheckCanContinueInning(const FName& UserName) const
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		return PlayerState->GetOutCount() < GetMaxOutCount();
	}

	return false;
}

FString ANBGameMode::GenerateComputerNumber() const
{
	TArray<int32> ValidRangeNumbers;
	for (int32 i = 1; i <= 9; i++)
	{
		ValidRangeNumbers.Add(i);
	}

	// 랜덤 초기화
	FMath::RandInit(FDateTime::Now().GetTicks());

	FString GeneratedResult;
	for (int32 i = 0; i < 3; i++)
	{
		const int32 Index = FMath::RandRange(0, ValidRangeNumbers.Num() - 1);
		GeneratedResult.Append(FString::FromInt(ValidRangeNumbers[Index]));
		ValidRangeNumbers.RemoveAt(Index);
	}
	
	return GeneratedResult;
}

FString ANBGameMode::JudgePlayResult(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState) return TEXT("Invalid Player");

	FString UserPitch = PlayerState->GetUserPitchString();
	FString ComputerPitch = PlayerState->GetComputerPitchString();

	uint8 StrikeCount = 0, BallCount = 0;
	int32 HitCount = 0;

	for (int32 i = 0; i < 3; i++)
	{
		if (UserPitch[i] == ComputerPitch[i])
		{
			HitCount++;
		}
		else if (ComputerPitch.Contains(FString::Chr(UserPitch[i])))
		{
			BallCount++;
		}
		else
		{
			StrikeCount++;
		}
	}

	for (int32 i = 0; i < HitCount; i++)
	{
		AddHitCount(UserName);
	}

	for (int32 i = 0; i < StrikeCount; i++)
	{
		if (!AddStrikeCount(UserName))
		{
			return FString::Printf(TEXT("%d Strike, %d Ball, %d Hit"), StrikeCount, BallCount, HitCount); 
		}
	}

	for (int32 i = 0; i < BallCount; i++)
	{
		if (!AddBallCount(UserName))
		{
			return FString::Printf(TEXT("%d Strike, %d Ball, %d Hit"), StrikeCount, BallCount, HitCount); 
		}
	}
	return FString::Printf(TEXT("%d Strike, %d Ball, %d Hit"), StrikeCount, BallCount, HitCount); 
}

void ANBGameMode::InitNBGame(const APlayerController* NewPlayer)
{
	auto* PlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (PlayerController)
	{
		auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
		if (PlayerState)
		{
			PlayerState->RequestGenerateComputerPitch();
		}
	}
}

void ANBGameMode::EndNBGame(const FName& UserName)
{

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
	
}

bool ANBGameMode::AddInningCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState) return false;

	uint8 Inning = PlayerState->GetInningCount() + 1;
	if (Inning > GetMaxInning())
	{
		EndNBGame(UserName);
		return false;
	}

	PlayerState->SetInningCount(Inning);
	AddGameScore(UserName);

	ResetOutCount(UserName);
	ResetStrikeCount(UserName);
	ResetBallCount(UserName);
	ResetHitCount(UserName);

	PlayerState->RequestGenerateComputerPitch();

	return true;
}

bool ANBGameMode::AddOutCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState) return false;

	uint8 NewOutCount = PlayerState->GetOutCount() + 1;

	PlayerState->SetOutCount(NewOutCount);
	ResetStrikeCount(UserName);
	ResetBallCount(UserName);

	if (NewOutCount >= GetMaxOutCount())
	{
		AddInningCount(UserName);
		return false;
	}

	return true;
}

bool ANBGameMode::AddStrikeCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState) return false;

	const uint8 NewStrikeCount = PlayerState->GetStrikeCount() + 1;

	if (NewStrikeCount >= GetMaxStrikeCount())
	{
		return AddOutCount(UserName);
	}

	PlayerState->SetStrikeCount(NewStrikeCount);
	return true;
}

bool ANBGameMode::AddBallCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState) return false;

	const uint8 NewBallCount = PlayerState->GetBallCount() + 1;

	if (NewBallCount >= GetMaxBallCount())
	{
		return AddStrikeCount(UserName);
	}

	PlayerState->SetBallCount(NewBallCount);
	return true;
}

void ANBGameMode::AddHitCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const int32 HitCount = PlayerState->GetHitCount();
		PlayerState->SetHitCount(HitCount+1);
	}

	ResetStrikeCount(UserName);
	ResetBallCount(UserName);
}

void ANBGameMode::AddGameScore(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const int32 HitCount = PlayerState->GetHitCount();
		const int32 AwardScore = FMath::Max(0, HitCount-3);
		const int32 CurrentScore = PlayerState->GetGameScore();
		PlayerState->SetGameScore(AwardScore + CurrentScore);
	}
}

void ANBGameMode::ResetBallCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		PlayerState->SetBallCount(0);
	}
}

void ANBGameMode::ResetStrikeCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		PlayerState->SetStrikeCount(0);
	}
}

void ANBGameMode::ResetOutCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		PlayerState->SetOutCount(0);
	}
}

void ANBGameMode::ResetHitCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		PlayerState->SetHitCount(0);
	}
}

void ANBGameMode::AssignDefaultUserName(const APlayerController* NewPlayer)
{
	int32 ClientConnectionCount;
	auto* NetDriver = NewPlayer->GetNetDriver();
	if (NetDriver)
	{
		ClientConnectionCount = NetDriver->ClientConnections.Num();
	}
	else
	{
		ClientConnectionCount = 0;
	}
	const int32 PlayerCount = ClientConnectionCount + 1;
	
	auto* PlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (PlayerController)
	{
		auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
		if (PlayerState)
		{
			const FName UserName = FName(FString::Printf(TEXT("Player%d"), PlayerCount));
			PlayerState->SetUserName(UserName);
			NB_LOG(LogBaseBall, Log, TEXT("UserName:%s"), *PlayerState->GetUserName().ToString());
		}
	}
}

void ANBGameMode::AddPlayerStatesToMap(const APlayerController* NewPlayer)
{
	auto* PlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (PlayerController)
	{
		auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
		if (PlayerState)
		{
			const FName UserName = PlayerState->GetUserName();
			PlayerStates.Add(UserName, PlayerState);
		}
	}
}

void ANBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AssignDefaultUserName(NewPlayer);
	AddPlayerStatesToMap(NewPlayer);
	InitNBGame(NewPlayer);
}


