// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerState.h"



ANBPlayerState::ANBPlayerState()
{
	bOnlyRelevantToOwner = true;
	
	UserName = FName(FString::Printf(TEXT("")));
	TurnCount = 1;
	PlayerInputValue = TEXT("");
	WinScore = 0;
	
}



