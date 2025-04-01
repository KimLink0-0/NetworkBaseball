// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBHUDWidget.generated.h"

class UNBProgressWidget;
class UNBScoreWidget;

UCLASS()
class NETWORKBASEBALL_API UNBHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBProgressWidget> ProgressWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNBScoreWidget> ScoreWidget;

	virtual void NativeConstruct() override;
};
