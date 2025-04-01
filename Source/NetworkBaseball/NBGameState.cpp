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
}

void ANBGameState::AddChatLog(const FString& NewChat)
{
	
	ServerChatLog.Add(NewChat);
	RequestUpdateChatLog();
	
}

void ANBGameState::AddProgressLog(const FString& NewProgress)
{


	ServerProgressLog.Add(NewProgress);
	RequestUpdateProgressLog();
	RequestUpdateScoreIcons();
	TimerResetScoreIcons();
	
	
}


void ANBGameState::ResetScoreIcons() const
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		CurrentInstancePlayer->ResetScoreIcons();
	}
}

void ANBGameState::TimerResetScoreIcons() const
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
	TimerResetScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}


void ANBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBGameState, StrikeCount);
	DOREPLIFETIME(ANBGameState, BallCount);
	DOREPLIFETIME(ANBGameState, OutCount);
	DOREPLIFETIME(ANBGameState, TurnCount);
	DOREPLIFETIME(ANBGameState, ServerChatLog);
	DOREPLIFETIME(ANBGameState, ServerProgressLog);
}
