// Fill out your copyright notice in the Description page of Project Settings.


#include "NBProgressWidget.h"

#include "NBGameState.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UNBProgressWidget::ResetInputMessage() const
{
	EditableTextWidget->SetText(FText::FromString(TEXT("")));
}

bool UNBProgressWidget::IsChatInput(const FText& NewInput) const
{
	const FString InputToString = NewInput.ToString();
	if (InputToString[0] != TEXT('/') || InputToString.Len() != 4)
	{
		return true;
	}
	
	TArray<int32> ValidRangeNumbers;
	for (int32 i = 1; i <= 9; i++)
	{
		ValidRangeNumbers.Add(i);
	}

	for (int32 i = 1; i < 4; i++)
	{
		FString ExtractedChar = InputToString.Mid(i,1);
		int32 Number = FCString::Atoi(*ExtractedChar);
		if (!ValidRangeNumbers.Contains(Number))
		{
			return true;
		}
	}
	return false;
}

void UNBProgressWidget::SendChatLog(const FText& InputText)
{
	const auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!NBPlayerController)
	{
		return;
	}
	const auto* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
	if (!NBPlayerState)
	{
		return;
	}
	
	const FString MessageToString = InputText.ToString();
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *NBPlayerState->GetUserName().ToString(), *MessageToString);
		NBGameState->AddChatLog(MessageToSend);
	}
}

void UNBProgressWidget::SendProgressLog(const FText& InputText)
{
	const auto* NBPlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!NBPlayerController)
	{
		return;
	}
	const auto* NBPlayerState = NBPlayerController->GetPlayerState<ANBPlayerState>();
	if (!NBPlayerState)
	{
		return;
	}
	
	const FString MessageToString = InputText.ToString();
	auto* NBGameState = GetWorld()->GetGameState<ANBGameState>();
	if (NBGameState)
	{
		const FString MessageToSend = FString::Printf(TEXT("[%s]:%s"), *NBPlayerState->GetUserName().ToString(), *MessageToString);
		NBGameState->AddProgressLog(MessageToSend);
	}
}

void UNBProgressWidget::SendMessageOnEnter(const FText& MessageFormText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !MessageFormText.IsEmpty())
	{
		if (IsChatInput(MessageFormText))
		{
			SendChatLog(MessageFormText);
		}
		else
		{
			SendProgressLog(MessageFormText);
		}
		
		ResetInputMessage();
	}
}

void UNBProgressWidget::SendMessageOnClick()
{
	const FText MessageFormText = EditableTextWidget->GetText();
	if (!MessageFormText.IsEmpty())
	{
		if (IsChatInput(MessageFormText))
		{
			SendChatLog(MessageFormText);
		}
		else
		{
			SendProgressLog(MessageFormText);
		}
		ResetInputMessage();
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
