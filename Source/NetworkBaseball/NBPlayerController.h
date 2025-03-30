// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBPlayerController.generated.h"

class UHUDWidget;

UCLASS()
class NETWORKBASEBALL_API ANBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANBPlayerController();

	
protected:
	// Widget
	TSoftClassPtr<UHUDWidget> HUDWidgetClass;
	TObjectPtr<UHUDWidget> HUDWidgetInstance;

	//
	void InitWidget();

	// LifeCycle
	virtual void BeginPlay() override;
	
};
