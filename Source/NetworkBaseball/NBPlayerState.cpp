// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerState.h"

#include "NBPlayerController.h"
#include "NetworkBaseball.h"
#include "Net/UnrealNetwork.h"


ANBPlayerState::ANBPlayerState()
{
	bOnlyRelevantToOwner = true;
	
	UserName = FName(FString::Printf(TEXT("")));
	TurnCount = 0;
	PlayerInputValue = TEXT("");
	WinScore = 0;
	GameCount = 1;

}


void ANBPlayerState::OnRep_UserName()
{
}

void ANBPlayerState::SetGameCount(const uint8& NewGameCount)
{
	GameCount = NewGameCount;
}

void ANBPlayerState::RequestNextGame()
{
	auto* NBPlayerController = Cast<ANBPlayerController>(GetPlayerController());
	if (NBPlayerController)
	{
		NBPlayerController->ServerRPCRequestNextGame(UserName);
	}
}

void ANBPlayerState::OnRep_WinScore()
{
	
}

void ANBPlayerState::OnRep_GameCount()
{
	
}

void ANBPlayerState::OnRep_TurnCount()
{
	if (TurnCount == 3)
	{
		RequestNextGame();
	}
}


void ANBPlayerState::BeginPlay()
{
	Super::BeginPlay();

	NB_LOG(LogBaseBall, Log, TEXT("UserName:%s"), *GetUserName().ToString())
}

void ANBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANBPlayerState, UserName);
	DOREPLIFETIME(ANBPlayerState, GameCount);
	DOREPLIFETIME(ANBPlayerState, WinScore);
	DOREPLIFETIME(ANBPlayerState, TurnCount);
}



