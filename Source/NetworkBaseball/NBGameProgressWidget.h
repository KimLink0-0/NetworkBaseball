// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NBGameProgressWidget.generated.h"

class UTextBlock;
class UEditableText;
class UButton;

UCLASS()
class NETWORKBASEBALL_API UNBGameProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	const FString& GetInputMessage() const { return InputMessage; }
	
	void ResetInputMessage() const;
	void UpdateInputMessageLog();
	void UpdateComputerPitch() const;
	void AddInputMessage(const FString& NewMessage);
	bool JudgePlayerPitch(const FString& NewtMessage);

	void UpdateDisplayInfo();
	
	UFUNCTION()
	void SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void SendMessageOnClick();
	

protected:
	FString InputMessage;
	TArray<FString> InputMessages;
	FString InputMessageLog;
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ComputerPitch;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UserInput;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> EditableText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SendButton;
	
};
