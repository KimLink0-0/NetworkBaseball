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
		int32 Index = FMath::RandRange(0, ValidRangeNumbers.Num() - 1);
		GeneratedResult.Append(FString::FromInt(ValidRangeNumbers[Index]));
		ValidRangeNumbers.RemoveAt(Index);
	}
	
	return GeneratedResult;
}

FString ANBGameMode::JudgePlayResult(const FName& PlayerName)
{
	uint8 StrikeCount = 0, BallCount = 0;
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		FString UserPitchString = PlayerState->GetUserPitchString();
		FString ComputerPitchString = PlayerState->GetComputerPitchString();
	
		for (int32 i = 0; i < 3; ++i)
		{
			if (ComputerPitchString[i] == UserPitchString[i])
			{
				StrikeCount++;
			}
			else
			{
				const FString UserNumber = FString::Chr(UserPitchString[i]);
				if (ComputerPitchString.Contains(UserNumber))
				{
					BallCount++;
				}
			}
		}

		for (int32 i = 0 ; i < StrikeCount ; i++)
		{
			if (CheckCanContinueInning(PlayerName))
			{
				AddStrikeCount(PlayerName);
			}
			else
			{
				break;
			}
		}
		for (int32 i = 0 ; i < BallCount ; i++)
		{
			if (CheckCanContinueInning(PlayerName))
			{
				AddBallCount(PlayerName);	
			}
			else
			{
				break;
			}
		}

		const uint8 StrikeCountResult = PlayerState->GetStrikeCount();
		const uint8 BallCountResult = PlayerState->GetBallCount();
		return FString::Printf(TEXT("%d Strike, %d Ball"), StrikeCountResult, BallCountResult);
	}
	else
	{
		return TEXT("Failed to JudgePlayResult");
	}
}

FString ANBGameMode::EndGame()
{
	FString GameResult;

	GameResult = TEXT("Game Over");
	
	return GameResult;
}

void ANBGameMode::AddInningCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		const uint8 InningCount = PlayerState->GetInningCount();
		if (InningCount == MaxInning)
		{
			EndGame();
		}
		else
		{
			PlayerState->SetInningCount(InningCount + 1);
		}
	}
	ResetOutCount(PlayerName); 
}

void ANBGameMode::AddOutCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		const uint8 OutCount = PlayerState->GetOutCount();
		if (OutCount == GetMaxOutCount())
		{
			AddInningCount(PlayerName);
			ResetOutCount(PlayerName);
		}
		else
		{
			
			PlayerState->SetOutCount(OutCount + 1);
		}
	}
}

void ANBGameMode::AddStrikeCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		const uint8 StrikeCount = PlayerState->GetStrikeCount();
		if (StrikeCount == GetMaxStrikeCount())
		{
			AddOutCount(PlayerName);
			ResetStrikeCount(PlayerName);
		}
		else
		{
			
			PlayerState->SetStrikeCount(StrikeCount + 1);
		}
	}
}

void ANBGameMode::AddBallCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		const uint8 BallCount = PlayerState->GetBallCount();
		if (BallCount == GetMaxBallCount())
		{
			AddStrikeCount(PlayerName);
			ResetBallCount(PlayerName);
		}
		else
		{
			
			PlayerState->SetBallCount(BallCount + 1);
		}
	}
}

void ANBGameMode::ResetBallCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		PlayerState->SetBallCount(0);
	}
}

void ANBGameMode::ResetStrikeCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		PlayerState->SetStrikeCount(0);
	}
}

void ANBGameMode::ResetOutCount(const FName& PlayerName)
{
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState)
	{
		PlayerState->SetOutCount(0);
	}
}

bool ANBGameMode::CheckCanContinueInning(const FName& PlayerName) const
{
	bool bCanContinue;
	
	auto* PlayerState = GetPlayerStates(PlayerName);
	if (PlayerState->GetOutCount() == GetMaxOutCount())
	{
		bCanContinue = false;
	}
	else
	{
		bCanContinue = true;
	}

	return bCanContinue;
}

void ANBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

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


