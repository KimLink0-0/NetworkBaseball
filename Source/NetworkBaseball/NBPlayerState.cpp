// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerState.h"

#include "NetworkBaseball.h"
#include "Net/UnrealNetwork.h"


ANBPlayerState::ANBPlayerState()
{
	bOnlyRelevantToOwner = true;
	
	UserName = FName(FString::Printf(TEXT("")));
	TurnCount = 1;
	PlayerInputValue = TEXT("");
	WinScore = 0;
	
}

void ANBPlayerState::OnRep_UserName()
{
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
}



