// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameProgressWidget.h"

#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UNBGameProgressWidget::ResetInputMessage() const
{
	EditableText->SetText(FText::FromString(TEXT("")));
}

void UNBGameProgressWidget::SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !MessageFormText.IsEmpty())
	{
		auto* PlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
		if (!PlayerController)
		{
			return;
		}
		auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
		if (!PlayerState)
		{
			return;
		}

		const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerState->GetUserName().ToString(), *MessageFormText.ToString());
		PlayerController->SendMessageToNetwork(MessageToSend);
		ResetInputMessage();
	}
}

void UNBGameProgressWidget::SendMessageOnClick()
{
	auto* PlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController)
	{
		return;
	}
	auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
	if (!PlayerState)
	{
		return;
	}

	const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerState->GetUserName().ToString(), *EditableText->GetText().ToString());
	if (!MessageToSend.IsEmpty())
	{
		PlayerController->SendMessageToNetwork(MessageToSend);
		ResetInputMessage();
	}
}

void UNBGameProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(ComputerPitch);
	ensure(UserInput);
	ensure(EditableText);

	// 이벤트 바인딩
	if (EditableText)
	{
		// if SetText = Hi, Failed to send login message.
		EditableText->SetText(FText::FromString(TEXT("Hi")));
		EditableText->OnTextCommitted.AddDynamic(this, &UNBGameProgressWidget::SendMessageOnEnter);
	}

	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UNBGameProgressWidget::SendMessageOnClick);
	}
}
