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

	// Widget 
	void UpdateProgressLog() const;
	void UpdateScoreIcons() const;
	void UpdateChatLog() const;
	void UpdateScoreText() const;
	void ResetScoreIcons();
	void ResetTurnCount();
	
	void SendMessageToGameMode(const FName UserName, const FText& MessageText) const;
	void CleanInputTextBox() const;

	// RPC
	UFUNCTION(Server, Reliable)
	void ServerRPCSendMessage(const FName UserName, const FText& MessageText);
	UFUNCTION(Client, Reliable)
	void ClientRPCRequestCleanInputTextBox();
	UFUNCTION(Server, Reliable)
	void ServerRPCRequestNextGame(const FName UserName);
	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateScoreText();
	
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
