// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UNBHUDWidget;

UCLASS()
class NETWORKBASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBPlayerController();

	TObjectPtr<UNBHUDWidget> GetHUDWidgetInstance() const { return HUDWidgetInstance; }

	void UpdateScoreIcons() const;
	void UpdateChatLog() const;
	void UpdateProgressLog() const;

	// RPC
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestUpdateScoreIcons();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUpdateScoreIcons();
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestUpdateProgressLog();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUpdateProgressLog();
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestUpdateChatLog();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCUpdateChatLog();
	
protected:
	// Widget
	void NewWidget();
	void InitWidget() const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UNBHUDWidget> HUDWidgetInstance;
	

	// LifeCycle
	virtual void BeginPlay() override;
	
};
