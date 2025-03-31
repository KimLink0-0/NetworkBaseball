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
	
protected:
	// Widget
	void NewWidget();
	void InitWidget();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr<UNBHUDWidget> HUDWidgetInstance;
	

	// LifeCycle
	virtual void BeginPlay() override;
	
};
