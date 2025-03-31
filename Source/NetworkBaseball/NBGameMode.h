// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NBGameMode.generated.h"

class ANBPlayerState;

UCLASS()
class NETWORKBASEBALL_API ANBGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANBGameMode();

	// Getter & Setter
	ANBPlayerState* GetPlayerStates(const FName& UserName) const { return PlayerStates.FindRef(UserName); }

	// Game Rule
	void GenerateComputerNumber() const;
	FString JudgePlayResult(const FName& UserName);
	
	// Add Player
	void AssignDefaultUserName(const APlayerController* NewPlayer);
	void AddPlayerStatesToMap(const APlayerController* NewPlayer);

protected:
	TMap<FName, ANBPlayerState*> PlayerStates;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

};



