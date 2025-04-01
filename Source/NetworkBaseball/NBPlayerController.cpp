// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

#include "NBGameMode.h"
#include "NBGameState.h"
#include "NBHUDWidget.h"
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
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBProgressWidget* ProgressWidget = HUDWidget->ProgressWidget;
		if (ProgressWidget)
		{
			ProgressWidget->UpdateProgressLog();
		}	
	}
	

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateScoreIcons() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBScoreWidget* ScoreWidget = HUDWidget->ScoreWidget;
		if (ScoreWidget)
		{
			ScoreWidget->UpdateScreen();
		}	
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateChatLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UNBHUDWidget* HUDWidget = GetHUDWidgetInstance();
	if (HUDWidget)
	{
		UNBProgressWidget* ProgressWidget = HUDWidget->ProgressWidget;
		if (ProgressWidget)
		{
			ProgressWidget->UpdateChatLog();
		}	
	}
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ResetScoreIcons()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
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
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	auto* NBGameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (NBGameMode)
	{
		NBGameMode->ReceivedInputMessage(UserName, MessageText);
	}
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::CleanInputTextBox() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

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
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}


void ANBPlayerController::ServerRPCSendMessage_Implementation(const FName UserName, const FText& MessageText)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	SendMessageToGameMode(UserName, MessageText);
	ClientRPCRequestCleanInputTextBox();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ClientRPCRequestCleanInputTextBox_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	CleanInputTextBox();

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
	
	

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::BeginPlay()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	Super::BeginPlay();

	if (IsLocalController())
	{
		NewWidget();
		InitWidget();
	}
	bShowMouseCursor = true;

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}
