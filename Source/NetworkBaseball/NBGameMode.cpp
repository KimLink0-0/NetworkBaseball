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
	auto* GameState = GetGameState<ANBGameState>();
	if (GameState)
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
		GameState->SetComputerNumber(GeneratedResult);
	}
}

FString ANBGameMode::JudgePlayResult(const FName& UserName)
{
	auto* PlayerState = GetPlayerStates(UserName);
	if (!PlayerState)
	{
		return FString::Printf(TEXT("%s not found %s"), *PlayerState->GetUserName().ToString(), *PlayerState->GetName());
	}
	auto* GameState = GetGameState<ANBGameState>();
	if (!GameState)
	{
		return FString::Printf(TEXT("%s not found %s"), *PlayerState->GetUserName().ToString(), *GameState->GetName());
	}
	
	uint8 Strike = 0, Ball = 0, Out = 0;
		
	for (int32 i = 0; i < 3; ++i)
	{
		FString PlayerNumber = PlayerState->GetPlayerInputValue();
		FString ComputerNumber = GameState->GetComputerNumber();
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

	GameState->SetStrikeCount(Strike);
	GameState->SetBallCount(Ball);
	GameState->SetOutCount(Out);

	// 승리 조건
	if (Strike == 3)
	{
		const uint8 WinScore = PlayerState->GetWinScore();
		PlayerState->SetWinScore(WinScore+1);
		return FString::Printf(TEXT("%d Strike, %d Ball, Winner is %s "), Strike, Ball, *UserName.ToString());	
	}
		
	return FString::Printf(TEXT("%d Strike, %d Ball"), Strike, Ball);
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


