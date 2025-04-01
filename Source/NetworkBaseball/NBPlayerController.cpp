// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

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


void ANBPlayerController::UpdateScoreIcons() const
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
				ScoreWidget->UpdateScreen();
			}	
		}
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateChatLog() const
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
				ProgressWidget->UpdateChatLog();
			}	
		}
	}
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateProgressLog() const
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
				ProgressWidget->UpdateChatLog();
			}	
		}
	}


	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ReceivedMessageMessage(const FText& MessageText) const
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
				ProgressWidget->ReceivedRPCOnEvent(MessageText);
			}	
		}
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

void ANBPlayerController::MulticastRPCUpdateScoreIcons_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UpdateScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCRequestUpdateScoreIcons_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	UpdateScoreIcons();
	MulticastRPCUpdateScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::MulticastRPCUpdateProgressLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UpdateProgressLog();
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCRequestUpdateProgressLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	UpdateProgressLog();
	MulticastRPCUpdateProgressLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCRequestUpdateChatLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	UpdateChatLog();
	MulticastRPCUpdateChatLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::MulticastRPCUpdateChatLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UpdateChatLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCSendMessage_Implementation(const FText& MessageText)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));

	ReceivedMessageMessage(MessageText);
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
	
	UpdateProgressLog();
	UpdateChatLog();
	UpdateScoreIcons();

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
