// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

#include "NBGameMode.h"
#include "NBGameState.h"
#include "NBHUDWidget.h"
#include "NBPlayerState.h"
#include "NBProgressWidget.h"
#include "NBScoreWidget.h"
#include "NetworkBaseball.h"

ANBPlayerController::ANBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(TEXT("/Game/Blueprints/WBP_NBHUDWidget.WBP_NBHUDWidget_c"));
	if (HUDWidget.Succeeded())
	{
		HUDWidgetClass = HUDWidget.Class;
	}
	HUDWidgetInstance = nullptr;
}

void ANBPlayerController::UpdateProgressLog() const
{
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBProgressWidget* ProgressWidget = HUDWidget->ProgressWidget;
		if (ProgressWidget)
		{
			ProgressWidget->UpdateProgressLog();
		}	
	}
	
}

void ANBPlayerController::UpdateScoreIcons() const
{
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBScoreWidget* ScoreWidget = HUDWidget->ScoreWidget;
		if (ScoreWidget)
		{
			ScoreWidget->UpdateScreen();
		}	
	}

}

void ANBPlayerController::UpdateChatLog() const
{
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBProgressWidget* ProgressWidget = HUDWidget->ProgressWidget;
		if (ProgressWidget)
		{
			ProgressWidget->UpdateChatLog();
		}	
	}
	
}

void ANBPlayerController::UpdateScoreText() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		UNBHUDWidget* HUDWidget = CurrentInstancePlayer->GetHUDWidgetInstance();
		if (HUDWidget)
		{
			UNBScoreWidget* ScoreWidget = HUDWidget->ScoreWidget;
			if (ScoreWidget)
			{
				ScoreWidget->UpdateScoreText();
			}	
		}
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ResetScoreIcons()
{
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBScoreWidget* ScoreWidget = HUDWidget->ScoreWidget;
		if (ScoreWidget)
		{
			ScoreWidget->ResetScreen();
		}	
	}
	
}

void ANBPlayerController::SendMessageToGameMode(const FName UserName, const FText& MessageText) const
{

	auto* NBGameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (NBGameMode)
	{
		ANBPlayerState* NBPlayerState = NBGameMode->GetPlayerStates(UserName);
		if (NBPlayerState)
		{
			NBPlayerState->SetPlayerInputValue(MessageText.ToString());
			NBGameMode->ReceivedInputMessage(UserName, MessageText);
		}
	}
	
}

void ANBPlayerController::CleanInputTextBox() const
{
	auto* CurrentInstancePlayer = Cast<ANBPlayerController>(GetWorld()->GetFirstPlayerController());
	if (CurrentInstancePlayer)
	{
		UNBHUDWidget* HUDWidget = CurrentInstancePlayer->GetHUDWidgetInstance();
		if (HUDWidget)
		{
			UNBProgressWidget* ProgressWidget = HUDWidget->ProgressWidget;
			if (ProgressWidget)
			{
				ProgressWidget->ResetInputTextBox();
			}	
		}
	}
	
}


void ANBPlayerController::ServerRPCSendMessage_Implementation(const FName UserName, const FText& MessageText)
{

	SendMessageToGameMode(UserName, MessageText);
	ClientRPCRequestCleanInputTextBox();

}

void ANBPlayerController::ClientRPCRequestCleanInputTextBox_Implementation()
{

	CleanInputTextBox();

}

void ANBPlayerController::ServerRPCRequestNextGame_Implementation(const FName UserName)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* NBGameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (NBGameMode)
	{
		NBGameMode->NextGame(UserName);
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::NewWidget()
{
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UNBHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

void ANBPlayerController::InitWidget() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	UpdateProgressLog();
	UpdateScoreIcons();
	UpdateChatLog();
	UpdateScoreText();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::BeginPlay()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::BeginPlay();

	if (IsLocalController())
	{
		NewWidget();

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			InitWidget();	
		}), 0.2f, false, -1);
		
	}
	bShowMouseCursor = true;

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}
