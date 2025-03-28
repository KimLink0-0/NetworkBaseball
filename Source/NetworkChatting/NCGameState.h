// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "NCGameState.generated.h"

UCLASS()
class NETWORKCHATTING_API ANCGameState : public AGameState
{
	GENERATED_BODY()

public:
	TArray<FString> GetServerChatLog() const;
	void AddServerChatLog(const FString& NewChatMessage);
	void UpdateServerChatLog() const;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_ServerChatLog)
	TArray<FString> ServerChatLog;

	UFUNCTION()
	void OnRep_ServerChatLog() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};