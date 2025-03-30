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

FString ANBGameMode::JudgePlayResult(const FName& UserName)
{
	uint8 StrikeCount = 0, BallCount = 0;
	int32 HitCount = 0;
	
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		FString UserPitchString = PlayerState->GetUserPitchString();
		FString ComputerPitchString = PlayerState->GetComputerPitchString();
	
		for (int32 i = 0; i < 3; ++i)
		{
			if (ComputerPitchString[i] == UserPitchString[i])
			{
				HitCount++;
			}
			else
			{
				const FString UserNumber = FString::Chr(UserPitchString[i]);
				if (ComputerPitchString.Contains(UserNumber))
				{
					StrikeCount++;
				}
				else
				{
					BallCount++;
				}
			}
		}
		for (int32 i = 0; i < HitCount; ++i)
		{
			if (CheckCanContinueInning(UserName))
			{
				AddHitCount(UserName);
			}
			else
			{
				break;
			}
		}

		for (int32 i = 0 ; i < StrikeCount ; i++)
		{
			if (CheckCanContinueInning(UserName))
			{
				AddStrikeCount(UserName);
			}
			else
			{
				break;
			}
		}
		for (int32 i = 0 ; i < BallCount ; i++)
		{
			if (CheckCanContinueInning(UserName))
			{
				AddBallCount(UserName);	
			}
			else
			{
				break;
			}
		}

		const uint8 StrikeCountResult = PlayerState->GetStrikeCount();
		const uint8 BallCountResult = PlayerState->GetBallCount();
		const int32 HitCountResult = PlayerState->GetHitCount();
		return FString::Printf(TEXT("%d Strike, %d Ball, %d HitCount"), StrikeCountResult, BallCountResult, HitCountResult);
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

void ANBGameMode::AddInningCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
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
	AddGameScore(UserName);
	ResetOutCount(UserName);
	ResetStrikeCount(UserName);
	ResetBallCount(UserName);
	ResetHitCount(UserName);
}

void ANBGameMode::AddOutCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const uint8 OutCount = PlayerState->GetOutCount();
		if (OutCount == GetMaxOutCount())
		{
			AddInningCount(UserName);
		}
		else
		{
			
			PlayerState->SetOutCount(OutCount + 1);
		}
	}
}

void ANBGameMode::AddStrikeCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const uint8 StrikeCount = PlayerState->GetStrikeCount();
		if (StrikeCount == GetMaxStrikeCount())
		{
			AddOutCount(UserName);
			ResetStrikeCount(UserName);
		}
		else
		{
			
			PlayerState->SetStrikeCount(StrikeCount + 1);
		}
	}
}

void ANBGameMode::AddBallCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const uint8 BallCount = PlayerState->GetBallCount();
		if (BallCount == GetMaxBallCount())
		{
			AddStrikeCount(UserName);
			ResetBallCount(UserName);
		}
		else
		{
			
			PlayerState->SetBallCount(BallCount + 1);
		}
	}
}

void ANBGameMode::AddHitCount(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const int32 HitCount = PlayerState->GetHitCount();
		PlayerState->SetHitCount(HitCount + 1);
	}
}

void ANBGameMode::AddGameScore(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		const int32 HitCount = PlayerState->GetHitCount();
		const int32 AwardScore = FMath::Max(0, HitCount-3);
		const int32 CurrentScore = PlayerState->GetGameScore();
		PlayerState->SetGameScore(CurrentScore + AwardScore);
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

bool ANBGameMode::CheckCanContinueInning(const FName& UserName) const
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (PlayerState)
	{
		return PlayerState->GetOutCount() < GetMaxOutCount();
	}

	return false;
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

}


