// Fill out your copyright notice in the Description page of Project Settings.


#include "NBSBOScreenWidget.h"

#include "NBPlayerState.h"
#include "Components/Image.h"

void UNBSBOScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StrikeIcons = { StrikeIcon1, StrikeIcon2, StrikeIcon3 };
	BallIcons = { BallIcon1, BallIcon2, BallIcon3, BallIcon4 };
	OutIcons = { OutIcon1, OutIcon2, OutIcon3 };
}

void UNBSBOScreenWidget::UpdateScreen()
{
	const auto* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}
	const auto* PlayerState = PlayerController->GetPlayerState<ANBPlayerState>();
	if (!PlayerState)
	{
		return;
	}
	
	const uint8 StrikeCount = PlayerState->GetStrikeCount();
	const uint8 BallCount = PlayerState->GetBallCount();
	const uint8 OutCount = PlayerState->GetOutCount();
	
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


