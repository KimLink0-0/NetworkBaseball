// Fill out your copyright notice in the Description page of Project Settings.


#include "NBScoreWidget.h"

#include "NBGameState.h"
#include "Components/Image.h"

void UNBScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StrikeIcons = { StrikeIcon1, StrikeIcon2, StrikeIcon3 };
	BallIcons = { BallIcon1, BallIcon2, BallIcon3, BallIcon4 };
	OutIcons = { OutIcon1, OutIcon2, OutIcon3 };
}

void UNBScoreWidget::UpdateScreen()
{
	const auto* GameState = Cast<ANBGameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return;
	}
	
	const uint8 StrikeCount = GameState->GetStrikeCount();
	const uint8 BallCount = GameState->GetBallCount();
	const uint8 OutCount = GameState->GetOutCount();
	
	for (int32 i = 0; i < StrikeCount; i++)
	{
		StrikeIcons[i]->SetColorAndOpacity( i < StrikeCount ?
			FLinearColor(0.99, 0.84, 0.25, 1.0):
			FLinearColor(0.46, 0.46, 0.46, 1.0));
	}

	for (int32 i = 0; i < BallCount; i++)
	{
		BallIcons[i]->SetColorAndOpacity( i < BallCount ?
			FLinearColor(0.00, 0.82, 0.42, 1.0):
			FLinearColor(0.46, 0.46, 0.46, 1.0));
	}

	for (int32 i = 0; i < OutCount; i++)
	{
		OutIcons[i]->SetColorAndOpacity( i < OutCount ?
			FLinearColor(0.97, 0.19, 0.18, 1.0):
			FLinearColor(0.46, 0.46, 0.46, 1.0));
	}
}

void UNBScoreWidget::ResetScreen()
{
	for (int32 i = 0; i < StrikeIcons.Num(); i++)
	{
		StrikeIcons[i]->SetColorAndOpacity(FLinearColor(0.46, 0.46, 0.46, 1.0));
	}

	for (int32 i = 0; i < BallIcons.Num(); i++)
	{
		BallIcons[i]->SetColorAndOpacity(FLinearColor(0.46, 0.46, 0.46, 1.0));
	}

	for (int32 i = 0; i < OutIcons.Num(); i++)
	{
		OutIcons[i]->SetColorAndOpacity(FLinearColor(0.46, 0.46, 0.46, 1.0));
	}
}


