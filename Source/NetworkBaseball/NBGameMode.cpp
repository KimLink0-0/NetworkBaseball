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

void ANBGameMode::ReceivedInputMessage(const FName& UserName, const FText& NewInput)
{
	if (IsChatInput(NewInput))
	{
		SendChatToState(UserName, NewInput);
	}
	else
	{
		JudgePlay(UserName, NewInput);
		SendProgressToState(UserName, NewInput);
		GenerateComputerNumber();
	}
}

bool ANBGameMode::IsChatInput(const FText& NewInput) const
{
	const FString InputToString = NewInput.ToString();
	if (InputToString.Len() != 4 || !InputToString.StartsWith(TEXT("/")))
	{
		return true;
	}
	
	TArray<int32> ValidRangeNumbers;
	for (int32 i = 1; i <= 9; i++)
	{
		ValidRangeNumbers.Add(i);
	}

	for (int32 i = 1; i < 4; i++)
	{
		FString ExtractedChar = InputToString.Mid(i,1);
		int32 Number = FCString::Atoi(*ExtractedChar);
		if (!ValidRangeNumbers.Contains(Number))
		{
			return true;
		}
	}
	return false;
}

void ANBGameMode::SendChatToState(const FName& UserName, const FText& NewInput)
{
	if (NewInput.IsEmpty())
	{
		return;
	}
	const auto* NBPlayerState = GetPlayerStates(UserName);
	if (!NBPlayerState)
	{
		return;
	}
	
	const FString MessageToString = NewInput.ToString();
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *UserName.ToString(), *MessageToString);
		NBGameState->AddChatLog(MessageToSend);
	}
}

void ANBGameMode::SendProgressToState(const FName& UserName, const FText& NewInput)
{
	if (NewInput.IsEmpty())
	{
		return;
	}
	const auto* NBPlayerState = GetPlayerStates(UserName);
	if (!NBPlayerState)
	{
		return;
	}
	
	const FString MessageToString = NewInput.ToString().Mid(1);
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		FString ComputerNumber = NBGameState->GetComputerNumber();
		const FString MessageToSend = FString::Printf(TEXT("[%s]:%s vs %s"), *UserName.ToString(), *MessageToString, *ComputerNumber);
		NBGameState->AddProgressLog(MessageToSend);
	}
}

FString ANBGameMode::JudgePlayResult(const FName& UserName, const FText& NewInput) const
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
		FString ComputerNumber = NBGameState->GetComputerNumber();
		if (!NewInput.IsEmpty() || !ComputerNumber.IsEmpty())
		{
			FString PlayerNumber = NewInput.ToString().Mid(1);
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
			}
		}
	}
	
	if (Strike == 0 && Ball == 0)
	{
		Out = 1;
	}

	NBGameState->SetStrikeCount(Strike);
	NBGameState->SetBallCount(Ball);
	NBGameState->SetOutCount(Out);

	// 승리 조건
	if (Strike == 3)
	{
		const uint8 WinScore = NBPlayerState->GetWinScore();
		NBPlayerState->SetWinScore(WinScore+1);
		NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("%d Strike, %d Ball, Winner is %s "), Strike, Ball, *UserName.ToString());
		return FString::Printf(TEXT("%d Strike, %d Ball, Winner is %s "), Strike, Ball, *UserName.ToString());	
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("%d Strike, %d Ball"), Strike, Ball);
	return FString::Printf(TEXT("%d Strike, %d Ball"), Strike, Ball);
}

void ANBGameMode::JudgePlay(const FName& UserName, const FText& NewInput) const
{
	const FString Result = JudgePlayResult(UserName, NewInput);
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

void ANBGameMode::BeginPlay()
{
	Super::BeginPlay();

	GenerateComputerNumber();
}


