// Fill out your copyright notice in the Description page of Project Settings.


#include "NBProgressWidget.h"

#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UNBProgressWidget::ResetInputMessage() const
{
	EditableTextWidget->SetText(FText::FromString(TEXT("")));
}

void UNBProgressWidget::SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod)
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
		ResetInputMessage();
	}
}

void UNBProgressWidget::SendMessageOnClick()
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

	const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerState->GetUserName().ToString(), *EditableTextWidget->GetText().ToString());
	if (!MessageToSend.IsEmpty())
	{
		ResetInputMessage();
	}
}

void UNBProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(GameStatusWidget);
	ensure(ChatLogWidget);
	ensure(EditableTextWidget);
	ensure(SendButtonWidget);

	// 이벤트 바인딩
	if (EditableTextWidget)
	{
		EditableTextWidget->OnTextCommitted.AddDynamic(this, &UNBProgressWidget::SendMessageOnEnter);
	}

	if (SendButtonWidget)
	{
		SendButtonWidget->OnClicked.AddDynamic(this, &UNBProgressWidget::SendMessageOnClick);
	}
}
