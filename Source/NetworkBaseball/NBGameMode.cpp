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


