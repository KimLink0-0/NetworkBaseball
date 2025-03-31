// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBProgressWidget.generated.h"

class UTextBlock;
class UEditableText;
class UButton;

UCLASS()
class NETWORKBASEBALL_API UNBProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ResetInputMessage() const;
	
	UFUNCTION()
	void SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void SendMessageOnClick();
	

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameStatusWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChatLogWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableTextWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SendButtonWidget;
};
