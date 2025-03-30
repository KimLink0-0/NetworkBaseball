// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerState.h"

#include "NBGameMode.h"


ANBPlayerState::ANBPlayerState()
{
	UserName = FName(FString::Printf(TEXT("")));
	InningCount = 1;
	StrikeCount = 0;
	BallCount = 0;
	OutCount = 0;
	UserPitchString = TEXT("");
	ComputerPitchString = TEXT("");
	
	bOnlyRelevantToOwner = true;
}

void ANBPlayerState::RequestGenerateComputerPitch()
{
	auto* GameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		FString GenResult = GameMode->GenerateComputerNumber();
		SetComputerPitchString(GenResult);
	}
}


