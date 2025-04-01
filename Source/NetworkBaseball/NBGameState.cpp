// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameState.h"

#include "NBPlayerController.h"
#include "NetworkBaseball.h"
#include "Net/UnrealNetwork.h"

ANBGameState::ANBGameState()
{
	StrikeCount = 0;
	BallCount = 0;
	OutCount = 0;
	TurnCount = 0;
	CurrentTurnComputerNumber = TEXT("");
	bCanProgress = true;
}

void ANBGameState::AddChatLog(const FString& NewChat)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	ServerChatLog.Add(NewChat);
	RequestUpdateChatLog();
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::AddProgressLog(const FString& NewProgress)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));


	ServerProgressLog.Add(NewProgress);
	RequestUpdateProgressLog();
	RequestUpdateScoreIcons();
	TimerResetScoreIcons();
	
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}


void ANBGameState::ResetScoreIcons()
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		CurrentInstancePlayer->ResetScoreIcons();
	}
}

void ANBGameState::TimerResetScoreIcons()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		ResetScoreIcons();
	}),3.0f, false, -1);
}

void ANBGameState::RequestUpdateChatLog() const
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		CurrentInstancePlayer->UpdateChatLog();
	}
}

void ANBGameState::RequestUpdateProgressLog() const
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		CurrentInstancePlayer->UpdateProgressLog();
	}
}

void ANBGameState::RequestUpdateScoreIcons() const
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		CurrentInstancePlayer->UpdateScoreIcons();
	}
}

void ANBGameState::OnRep_StrikeCount()
{
}

void ANBGameState::OnRep_BallCount()
{
}

void ANBGameState::OnRep_OutCount()
{
}

void ANBGameState::OnRep_TurnCount()
{
}

void ANBGameState::OnRep_ComputerNumber()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	TimerResetScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::OnRep_ChatLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	RequestUpdateChatLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::OnRep_ProgressLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	RequestUpdateProgressLog();
	RequestUpdateScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::OnRep_CanProgress()
{
}

void ANBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBGameState, StrikeCount);
	DOREPLIFETIME(ANBGameState, BallCount);
	DOREPLIFETIME(ANBGameState, OutCount);
	DOREPLIFETIME(ANBGameState, TurnCount);
	DOREPLIFETIME(ANBGameState, CurrentTurnComputerNumber);
	DOREPLIFETIME(ANBGameState, ServerChatLog);
	DOREPLIFETIME(ANBGameState, ServerProgressLog);
	DOREPLIFETIME(ANBGameState, bCanProgress);
}
