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

	// RPC 관련
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestSetUserPitch(const FString& Message);
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestJudgePlay(const FName& UserName);
	UFUNCTION(Client, Reliable)
	void ClientRPCRequestUpdateWidget();

	void RequestJudgePlay(const FName& UserName);
	void RequestUpdateScreen() const;
	void RequestUpdateProgress() const;
	void SendMessageToNetwork(const FString& MessageToSend);
	void SetUserPitch(const FString& Message);
	TObjectPtr<UNBHUDWidget> GetHUDWidgetInstance() const { return HUDWidgetInstance; }
	
protected:
	// Widget
	void InitWidget();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UNBHUDWidget> HUDWidgetInstance;
	

	// LifeCycle
	virtual void BeginPlay() override;
	
};
