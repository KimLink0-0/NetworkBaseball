// Fill out your copyright notice in the Description page of Project Settings.


#include "NBGameProgressWidget.h"

#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"

void UNBGameProgressWidget::ResetInputMessage() const
{
	EditableText->SetText(FText::FromString(TEXT("")));
}

void UNBGameProgressWidget::UpdateInputMessageLog()
{
	InputMessageLog = FString::Join(InputMessages, TEXT("\n"));
	UserInput->SetText(FText::FromString(InputMessageLog));
}

void UNBGameProgressWidget::UpdateComputerPitch() const
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

	FString ComputerPitchToString = PlayerState->GetComputerPitchString();
	FString DisplayText = FString::Printf(TEXT("PreComputerPitch:%s"), *ComputerPitchToString);
	ComputerPitch->SetText(FText::FromString(DisplayText));
}

void UNBGameProgressWidget::AddInputMessage(const FString& NewMessage)
{
	InputMessages.Add(NewMessage);
	UpdateDisplayInfo();
}

bool UNBGameProgressWidget::JudgePlayerPitch(const FString& NewMessage)
{
	if (NewMessage.Len() != 3)
	{
		return false;	
	}

	TArray<int32> ValidRangeNumbers;
	for (int32 i = 1; i <= 9; i++)
	{
		ValidRangeNumbers.Add(i);
	}
	
	for (int32 i = 0; i < 3; ++i)
	{
		FString ExtractedChar = NewMessage.Mid(i,1);
		int32 Number = FCString::Atoi(*ExtractedChar);
		if (!ValidRangeNumbers.Contains(Number))
		{
			return false;
		}
	}

	return true;
}

void UNBGameProgressWidget::UpdateDisplayInfo()
{
	UpdateInputMessageLog();
	UpdateComputerPitch();
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

		FString MessageToSend;
		if (JudgePlayerPitch(MessageFormText.ToString()))
		{
			PlayerController->SendMessageToNetwork(MessageFormText.ToString());
			MessageToSend = FString::Printf(TEXT("[%sPitch:%s]"), *PlayerState->GetUserName().ToString(), *MessageFormText.ToString());
		}
		else
		{
			MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerState->GetUserName().ToString(), *MessageFormText.ToString());
		}
		
		AddInputMessage(MessageToSend);
		ResetInputMessage();
	}
}

void UNBGameProgressWidget::SendMessageOnClick()
{
	const FString InputMessageToString = EditableText->GetText().ToString();
	if (!InputMessageToString.IsEmpty())
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
	
		FString MessageToSend;
		if (JudgePlayerPitch(InputMessageToString))
		{
			PlayerController->SendMessageToNetwork(EditableText->GetText().ToString());
			MessageToSend = FString::Printf(TEXT("[%sPitch:%s]"), *PlayerState->GetUserName().ToString(), *InputMessageToString);
		}
		else
		{
			MessageToSend = FString::Printf(TEXT("[%s]:%s"), *PlayerState->GetUserName().ToString(), *InputMessageToString);	
		}
		
		AddInputMessage(MessageToSend);
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
		EditableText->OnTextCommitted.AddDynamic(this, &UNBGameProgressWidget::SendMessageOnEnter);
	}

	if (SendButton)
	{
		SendButton->OnClicked.AddDynamic(this, &UNBGameProgressWidget::SendMessageOnClick);
	}
}
