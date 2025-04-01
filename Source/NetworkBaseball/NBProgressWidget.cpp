// Fill out your copyright notice in the Description page of Project Settings.


#include "NBProgressWidget.h"

#include "NBGameState.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UNBProgressWidget::ResetInputTextBox() const
{
	EditableTextWidget->SetText(FText::FromString(TEXT("")));
}


void UNBProgressWidget::RequestRPCToServer(const FText& MessageFormText) const
{
	auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (NBPlayerController)
	{
		auto* PlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
		if (PlayerState)
		{
			FName UserName = PlayerState->GetUserName();
			NBPlayerController->ServerRPCSendMessage(UserName, MessageFormText);
		}
	}
}

void UNBProgressWidget::SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !MessageFormText.IsEmpty())
	{
		RequestRPCToServer(MessageFormText);
	}
}

void UNBProgressWidget::SendMessageOnClick()
{
	const FText MessageFormText = EditableTextWidget->GetText();
	if (!MessageFormText.IsEmpty())
	{
		RequestRPCToServer(MessageFormText);
	}
}

void UNBProgressWidget::UpdateProgressLog() const
{
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		const TArray<FString> ServerProgressLog = NBGameState->GetProgressLog();
		const FString ProgressLog = FString::Join(ServerProgressLog, TEXT("\n"));
		GameStatusWidget->SetText(FText::FromString(ProgressLog));
	}
}

void UNBProgressWidget::UpdateChatLog() const
{
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		const TArray<FString> ServerChatLog = NBGameState->GetChatLog();
		const FString ChatLog = FString::Join(ServerChatLog, TEXT("\n"));
		ChatLogWidget->SetText(FText::FromString(ChatLog));
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
