// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBScoreWidget.generated.h"

class UImage;

UCLASS()
class NETWORKBASEBALL_API UNBScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateScreen();
	void ResetScreen();

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StrikeIcon1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StrikeIcon2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StrikeIcon3;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BallIcon1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BallIcon2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BallIcon3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BallIcon4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OutIcon1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OutIcon2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OutIcon3;

	TArray<TObjectPtr<UImage>> StrikeIcons;
	TArray<TObjectPtr<UImage>> BallIcons;
	TArray<TObjectPtr<UImage>> OutIcons;
	
};
