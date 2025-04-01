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

void ANBGameMode::GenerateComputerNumber(const FName& UserName) const
{
	auto* NBPlayerState = GetPlayerStates(UserName);
	ensure(NBPlayerState);
	
	TArray<int32> ValidRangeNumbers;
	for (int32 i = 1; i <= 9; i++)
	{
		ValidRangeNumbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	
	FString GeneratedResult;
	for (int32 i = 0; i < 3; i++)
	{
		const int32 Index = FMath::RandRange(0, ValidRangeNumbers.Num() - 1);
		GeneratedResult.Append(FString::FromInt(ValidRangeNumbers[Index]));
		ValidRangeNumbers.RemoveAt(Index);
	}
	NBPlayerState->SetComputerGenNumber(GeneratedResult);
}


void ANBGameMode::StartTurn(const FName& UserName) const
{
	auto* PlayerState = GetPlayerStates(UserName);
	ensure(PlayerState);
	
	const FString NewInputToString = PlayerState->GetPlayerInputValue();
	const FText NewInput = FText::FromString(NewInputToString);
	
	JudgePlay(UserName, NewInput);
	SendProgressToState(UserName, NewInput);
}

void ANBGameMode::NextGame(const FName& UserName) const
{
	NB_LOG(LogBaseBall, Log, TEXT("NextGame 시작"));
	
	auto* PlayerState = GetPlayerStates(UserName);
	ensure(PlayerState);
	
	const uint8 CurrentGameCount = PlayerState->GetGameCount();
	
	PlayerState->SetTurnCount(0);
	PlayerState->SetGameCount(CurrentGameCount + 1);
	GenerateComputerNumber(UserName);

	NB_LOG(LogBaseBall, Log, TEXT("NextGame 종료"));
}

void ANBGameMode::ReceivedInputMessage(const FName& UserName, const FText& NewInput) const
{
	auto* PlayerState = GetPlayerStates(UserName);
	ensure(PlayerState);
	
	if (IsChatInput(NewInput))
	{
		SendChatToState(UserName, NewInput);
	}
	else
	{
		const uint8 TurnCount = PlayerState->GetTurnCount();
		const uint8 NewTurnCount = TurnCount + 1;
		if (TurnCount < MaxTurnCount)
		{
			PlayerState->SetTurnCount(NewTurnCount);
			StartTurn(UserName);
		}
		// if (NewTurnCount >= MaxTurnCount) 대신 else 사용하여 중복 호출 방지
		else
		{
			NextGame(UserName);
		}
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

void ANBGameMode::SendChatToState(const FName& UserName, const FText& NewInput) const
{
	if (NewInput.IsEmpty())
	{
		return;
	}
	
	const auto* NBPlayerState = GetPlayerStates(UserName);
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	
	ensure(NBPlayerState);
	ensure(NBGameState);
	
	const FString MessageToString = NewInput.ToString();
	const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *UserName.ToString(), *MessageToString);
	NBGameState->AddChatLog(MessageToSend);
}

void ANBGameMode::SendProgressToState(const FName& UserName, const FText& NewInput) const
{
	if (NewInput.IsEmpty())
	{
		return;
	}
	
	const auto* NBPlayerState = GetPlayerStates(UserName);
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	const FString MessageToString = NewInput.ToString().Mid(1);
	FString MessageToSend;
	
	ensure(NBPlayerState);
	ensure(NBGameState);

	if (NBPlayerState->GetTurnCount() < MaxTurnCount)
	{
		const uint8 LeftChance = MaxTurnCount - NBPlayerState->GetTurnCount();
		const FString LeftChanceToString = FString::FromInt(LeftChance);
		MessageToSend = FString::Printf(TEXT("[%s]:%s Chance:%s"), *UserName.ToString(), *MessageToString, *LeftChanceToString);
	}
	else
	{
		const FString ComputerNumber = NBPlayerState->GetComputerGenNumber();
		MessageToSend = FString::Printf(TEXT("[%s]:%s vs %s"), *UserName.ToString(), *MessageToString, *ComputerNumber);	
	}
	NBGameState->AddProgressLog(MessageToSend);
}

FString ANBGameMode::JudgePlayResult(const FName& UserName, const FText& NewInput) const
{
	auto* NBPlayerState = GetPlayerStates(UserName);
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	check(NBPlayerState);
	check(NBGameState);

	if (NBPlayerState->GetComputerGenNumber().IsEmpty())
	{
		GenerateComputerNumber(UserName);
	}
	
	uint8 Strike = 0, Ball = 0, Out = 0;
		
	for (int32 i = 0; i < 3; ++i)
	{
		FString ComputerNumber = NBPlayerState->GetComputerGenNumber();
		if (!NewInput.IsEmpty() && !ComputerNumber.IsEmpty())
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

	if (Strike == 3)
	{
		NBPlayerState->SetWinScore(NBPlayerState->GetWinScore() + 1);
		NB_LOG(LogBaseBall, Log, TEXT("%d Strike, %d Ball, Winner is %s"), Strike, Ball, *UserName.ToString());
		HandleGameOver(UserName, true);
		return FString::Printf(TEXT("%d Strike, %d Ball, Winner is %s"), Strike, Ball, *UserName.ToString());
	}

	if (Out == 1)
	{
		FName OtherPlayerName = FindOtherPlayerName(UserName);
		if (OtherPlayerName != NAME_None)
		{
			auto* OtherPlayerState = GetPlayerStates(OtherPlayerName);
			if (OtherPlayerState)
			{
				OtherPlayerState->SetWinScore(OtherPlayerState->GetWinScore() + 1);
				HandleGameOver(OtherPlayerName, true);
				return FString::Printf(TEXT("Player %s is out. Winner is %s"), *UserName.ToString(), *OtherPlayerName.ToString());
			}
		}
	}

	bool bAllPlayersDone = true;
	for (auto& PlayerState : PlayerStates)
	{
		if (PlayerState.Value->GetTurnCount() < MaxTurnCount)
		{
			bAllPlayersDone = false;
			break;
		}
	}
	if (bAllPlayersDone)
	{
		HandleGameOver(FName(TEXT("Draw")), false);
		return FString(TEXT("Draw"));
	}

	NB_LOG(LogBaseBall, Log, TEXT("%d Strike, %d Ball"), Strike, Ball);
	return FString::Printf(TEXT("%d Strike, %d Ball"), Strike, Ball);
}

void ANBGameMode::JudgePlay(const FName& UserName, const FText& NewInput) const
{
	const FString Result = JudgePlayResult(UserName, NewInput);
	BroadcastPlayResult(Result);
}

void ANBGameMode::BroadcastPlayResult(const FString& Result) const 
{
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		NBGameState->AddProgressLog(Result);
	}
}

// HandleGameOver: 게임 종료 시 승리/무승부 처리 후 모든 플레이어 상태 리셋
void ANBGameMode::HandleGameOver(const FName& WinnerUserName, bool bHasWinner) const
{
	if (bHasWinner)
	{
		NB_LOG(LogBaseBall, Log, TEXT("Winner is %s"), *WinnerUserName.ToString());
	}
	else
	{
		NB_LOG(LogBaseBall, Log, TEXT("Game is Draw"));
	}
	
	for (auto& PlayerState : PlayerStates)
	{
		PlayerState.Value->SetTurnCount(0);
	}
	
	for (auto& PlayerState : PlayerStates)
	{
		GenerateComputerNumber(PlayerState.Key);
	}
	
}

// FindOtherPlayerName: 현재 플레이어가 아닌 다른 플레이어의 이름 반환
FName ANBGameMode::FindOtherPlayerName(const FName& CurrentUserName) const
{
	for (auto& PlayerState : PlayerStates)
	{
		if (PlayerState.Key != CurrentUserName)
		{
			return PlayerState.Key;
		}
	}
	return NAME_None;
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
}
