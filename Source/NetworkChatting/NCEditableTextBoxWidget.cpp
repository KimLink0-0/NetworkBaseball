// Fill out your copyright notice in the Description page of Project Settings.


#include "NCEditableTextBoxWidget.h"

#include "NCHUDWidget.h"
#include "NCPlayerController.h"
#include "Components/Button.h"
#include "Components/EditableText.h"


const FString& UNCEditableTextBoxWidget::GetWittenMessage() const
{
	return SendMessageEditableText->GetText().ToString();
}

void UNCEditableTextBoxWidget::ResetWittenMessage() const
{
	SendMessageEditableText->SetText(FText::FromString(TEXT("")));
}

void UNCEditableTextBoxWidget::SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !MessageFormText.IsEmpty())
	{
		auto* PlayerController = Cast<ANCPlayerController>(GetWorld()->GetFirstPlayerController());
		if (!PlayerController)
		{
			return;
		}

		MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerController->GetUserName(), *MessageFormText.ToString());
		PlayerController->SendNetworkMessage(MessageToSend);
		ResetWittenMessage();
	}
}

void UNCEditableTextBoxWidget::SendMessageOnClick()
{
	auto* PlayerController = Cast<ANCPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController)
	{
		return;
	}

	MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerController->GetUserName(), *SendMessageEditableText->GetText().ToString());
	if (!MessageToSend.IsEmpty())
	{
		PlayerController->SendNetworkMessage(MessageToSend);
		ResetWittenMessage();
	}
}

void UNCEditableTextBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Blueprints 위젯 연결 확인
	ensure(SendMessageEditableText);
	ensure(SendButton);

	// 이벤트 바인딩
	if (SendMessageEditableText)
	{
		// if SetText = Hi, Failed to send login message.
		SendMessageEditableText->SetText(FText::FromString(TEXT("Hi")));
		SendMessageEditableText->OnTextCommitted.AddDynamic(this, &UNCEditableTextBoxWidget::SendMessageOnEnter);
	}

	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UNCEditableTextBoxWidget::SendMessageOnClick);
	}
}
