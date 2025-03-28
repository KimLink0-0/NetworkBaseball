// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCMessageBoxWidget.generated.h"



UCLASS()
class NETWORKCHATTING_API UNCMessageBoxWidget : public UNCUserWidget
{
	GENERATED_BODY()

public:
	void UpdateChattingLog(const TArray<FString>& NewChattingLog);
	void ShowChattingLog();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	FString MessagesForViewing;
	UPROPERTY()
	TArray<FString> CurrentChattingLog;

	// UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MessageTextBlock;
};

