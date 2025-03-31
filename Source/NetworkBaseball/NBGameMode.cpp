// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameMode.h"

#include "NBGameState.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "NetworkBaseball.h"

ANBGameMode::ANBGameMode()
{
	PlayerControllerClass = ANBPlayerController::StaticClass();
	GameStateClass = ANBGameState::StaticClass();
	PlayerStateClass = ANBPlayerState::StaticClass();
	
}

void ANBGameMode::GenerateComputerNumber() const
{
	auto* NBGameState = GetGameState<ANBGameState>();
	if (NBGameState)
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
		NBGameState->SetComputerNumber(GeneratedResult);
	}
}

FString ANBGameMode::JudgePlayResult(const FName& UserName) const
{
	auto* NBPlayerState = GetPlayerStates(UserName);
	if (!NBPlayerState)
	{
		return FString::Printf(TEXT("%s not found %s"), *NBPlayerState->GetUserName().ToString(), *NBPlayerState->GetName());
	}
	auto* NBGameState = GetGameState<ANBGameState>();
	if (!NBGameState)
	{
		return FString::Printf(TEXT("%s not found %s"), *NBPlayerState->GetUserName().ToString(), *NBGameState->GetName());
	}
	
	uint8 Strike = 0, Ball = 0, Out = 0;
		
	for (int32 i = 0; i < 3; ++i)
	{
		FString PlayerNumber = NBPlayerState->GetPlayerInputValue();
		FString ComputerNumber = NBGameState->GetComputerNumber();
		if (ComputerNumber[i] == PlayerNumber[i])
		{
			Strike++;
		}
		else
		{
			const FString UserNumber = FString::Chr(PlayerNumber[i]);
			if (ComputerNumber.Contains(UserNumber))
			{
				Ball++;
			}
			else
			{
				Out++;
			}
		}
	}

	NBGameState->SetStrikeCount(Strike);
	NBGameState->SetBallCount(Ball);
	NBGameState->SetOutCount(Out);

	// 승리 조건
	if (Strike == 3)
	{
		const uint8 WinScore = NBPlayerState->GetWinScore();
		NBPlayerState->SetWinScore(WinScore+1);
		return FString::Printf(TEXT("%d Strike, %d Ball, Winner is %s "), Strike, Ball, *UserName.ToString());	
	}
		
	return FString::Printf(TEXT("%d Strike, %d Ball"), Strike, Ball);
}

void ANBGameMode::JudgePlay(const FName& UserName) const
{
	const FString Result = JudgePlayResult(UserName);
	BroadcastPlayResult(Result);
}

void ANBGameMode::BroadcastPlayResult(const FString& Result) const 
{
	auto* NBGameState = GetGameState<ANBGameState>();
	if (NBGameState)
	{
		NBGameState->AddProgressLog(Result);
	}
}


void ANBGameMode::AssignDefaultUserName(const APlayerController* NewPlayer) const
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
	
	auto* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (NBPlayerController)
	{
		auto* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
		if (NBPlayerState)
		{
			const FName UserName = FName(FString::Printf(TEXT("Player%d"), PlayerCount));
			NBPlayerState->SetUserName(UserName);
			NB_LOG(LogBaseBall, Log, TEXT("UserName:%s"), *NBPlayerState->GetUserName().ToString());
		}
	}
}

void ANBGameMode::AddPlayerStatesToMap(const APlayerController* NewPlayer)
{
	auto* NBPlayerController = Cast<ANBPlayerController>(NewPlayer);
	if (NBPlayerController)
	{
		auto* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
		if (NBPlayerState)
		{
			const FName UserName = NBPlayerState->GetUserName();
			PlayerStates.Add(UserName, NBPlayerState);
		}
	}
}

void ANBGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AssignDefaultUserName(NewPlayer);
	AddPlayerStatesToMap(NewPlayer);

}


