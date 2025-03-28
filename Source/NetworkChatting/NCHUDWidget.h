// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NCHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETWORKCHATTING_API UNCHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UNCEditableTextBoxWidget> EditableTextBoxWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UNCMessageBoxWidget> MessageBoxWidget;

protected:
	virtual void NativeConstruct() override;
};
