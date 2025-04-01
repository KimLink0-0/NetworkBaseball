// Fill out your copyright notice in the Description page of Project Settings.

#include "NBPlayerState.h"

#include "NBPlayerController.h"
#include "NetworkBaseball.h"
#include "Net/UnrealNetwork.h"

ANBPlayerState::ANBPlayerState()
{
	bOnlyRelevantToOwner = true;
	
	UserName = FName(TEXT(""));
	TurnCount = 0;
	PlayerInputValue = TEXT("");
	WinScore = 0;
	GameCount = 1;
	ComputerGenNumber = TEXT("");
}

void ANBPlayerState::SetTurnCount(const uint8& NewTurnCount)
{
	TurnCount = NewTurnCount;
	UpdateScoreWidget();
}

void ANBPlayerState::SetGameCount(const uint8& NewGameCount)
{
	GameCount = NewGameCount;
	UpdateScoreWidget();
}

void ANBPlayerState::RequestNextGame()
{
	auto* NBPlayerController = Cast<ANBPlayerController>(GetPlayerController());
	if (NBPlayerController)
	{
		NBPlayerController->ServerRPCRequestNextGame(UserName);
	}
}

void ANBPlayerState::UpdateScoreWidget()
{
	if (GetOwningController() && GetOwningController()->IsLocalController())
	{
		auto* NBPlayerController = Cast<ANBPlayerController>(GetOwningController());
		ensure(NBPlayerController);
		NBPlayerController->UpdateScoreText();
	}
}

void ANBPlayerState::OnRep_UserName()
{
}

void ANBPlayerState::OnRep_WinScore()
{
}

void ANBPlayerState::OnRep_GameCount()
{
	UpdateScoreWidget();
}

void ANBPlayerState::OnRep_TurnCount()
{
	UpdateScoreWidget();
}

void ANBPlayerState::OnRep_GenNumber()
{
	UpdateScoreWidget();
}

void ANBPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	NB_LOG(LogBaseBall, Log, TEXT("UserName:%s"), *GetUserName().ToString());
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBPlayerState, UserName);
	DOREPLIFETIME(ANBPlayerState, GameCount);
	DOREPLIFETIME(ANBPlayerState, WinScore);
	DOREPLIFETIME(ANBPlayerState, TurnCount);
	DOREPLIFETIME(ANBPlayerState, ComputerGenNumber);
}
