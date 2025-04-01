// Fill out your copyright notice in the Description page of Project Settings.


#include "NBScoreWidget.h"

#include "NBGameState.h"
#include "NBPlayerController.h"
#include "NBPlayerState.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UNBScoreWidget::UpdateScreen()
{
	const auto* NBGameState = Cast<ANBGameState>(GetWorld()->GetGameState());
	if (!NBGameState)
	{
		return;
	}
	
	const uint8 StrikeCount = NBGameState->GetStrikeCount();
	const uint8 BallCount = NBGameState->GetBallCount();
	const uint8 OutCount = NBGameState->GetOutCount();
	
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

void UNBScoreWidget::UpdateScoreText()
{
	auto* PlayerController = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	ensure (PlayerController);
	
	auto* NBPlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
	ensure (NBPlayerState);
	
	const FString UserName = NBPlayerState->GetUserName().ToString();
	const uint8 GameCount = NBPlayerState->GetGameCount();
	const uint8 WinScore = NBPlayerState->GetWinScore();

	const FString CurrentScore = FString::Printf(TEXT("%s GameCount:%d WinScore:%d"), *UserName, GameCount, WinScore);
	const FText CurrentScoreToText = FText::FromString(CurrentScore);
	ScoreText->SetText(CurrentScoreToText);
	
}

void UNBScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ensure(ScoreText);
	ensure(StrikeIcon1);
	ensure(StrikeIcon2);
	ensure(StrikeIcon3);
	ensure(BallIcon1);
	ensure(BallIcon2);
	ensure(BallIcon3);
	ensure(BallIcon4);
	ensure(OutIcon1);
	ensure(OutIcon2);
	ensure(OutIcon3);

	StrikeIcons = { StrikeIcon1, StrikeIcon2, StrikeIcon3 };
	BallIcons = { BallIcon1, BallIcon2, BallIcon3, BallIcon4 };
	OutIcons = { OutIcon1, OutIcon2, OutIcon3 };
}


