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
}

void ANBGameState::RequestUpdateProgressLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (NBPlayerController)
	{
		NBPlayerController->ServerRPCRequestUpdateProgressLog();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::RequestUpdateScoreIcons() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (NBPlayerController)
	{
		NBPlayerController->ServerRPCRequestUpdateScoreIcons();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBGameState::RequestUpdateChatLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (NBPlayerController)
	{
		NBPlayerController->ServerRPCRequestUpdateChatLog();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
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
}
