// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameState.h"

ANBGameState::ANBGameState()
{
	StrikeCount = 0;
	BallCount = 0;
	OutCount = 0;
	TurnCount = 0;
	CurrentTurnComputerNumber = TEXT("");
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

void ANBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}
