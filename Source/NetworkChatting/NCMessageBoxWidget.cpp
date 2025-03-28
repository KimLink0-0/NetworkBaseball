// Fill out your copyright notice in the Description page of Project Settings.


#include "NCMessageBoxWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UNCMessageBoxWidget::UpdateChattingLog(const TArray<FString>& NewChattingLog)
{
	CurrentChattingLog = NewChattingLog;
	ShowChattingLog();
}

void UNCMessageBoxWidget::ShowChattingLog()
{
	MessagesForViewing = FString::Join(CurrentChattingLog, TEXT("\n"));
	MessageTextBlock->SetText(FText::FromString(MessagesForViewing));
}

void UNCMessageBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Blueprints 위젯 연결 확인
	ensure(MessageTextBlock);

	// for Add Client
	ShowChattingLog();
}


