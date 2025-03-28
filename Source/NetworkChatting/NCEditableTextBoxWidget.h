// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NCUserWidget.h"
#include "NCEditableTextBoxWidget.generated.h"


UCLASS()
class NETWORKCHATTING_API UNCEditableTextBoxWidget : public UNCUserWidget
{
	GENERATED_BODY()

public:
	const FString& GetWittenMessage() const;
	void ResetWittenMessage() const ;

	UFUNCTION()
	void SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void SendMessageOnClick();

protected:
	FString MessageToSend;
	
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> SendMessageEditableText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SendButton;
	
};
