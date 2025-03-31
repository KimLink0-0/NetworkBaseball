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
	
	auto* ScoreWidget = Cast<UNBScoreWidget>(GetHUDWidgetInstance());
	if (ScoreWidget)
	{
		ScoreWidget->UpdateScreen();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateChatLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* ProgressWidget = Cast<UNBProgressWidget>(GetHUDWidgetInstance());
	if (ProgressWidget)
	{
		ProgressWidget->UpdateChatLog();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::UpdateProgressLog() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	auto* ProgressWidget = Cast<UNBProgressWidget>(GetHUDWidgetInstance());
	if (ProgressWidget)
	{
		ProgressWidget->UpdateProgressLog();
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
	
	MulticastRPCUpdateScoreIcons();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCRequestUpdateProgressLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	MulticastRPCUpdateProgressLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::MulticastRPCUpdateProgressLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UpdateProgressLog();
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::ServerRPCRequestUpdateChatLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	MulticastRPCUpdateChatLog();

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"));
}

void ANBPlayerController::MulticastRPCUpdateChatLog_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"));
	
	UpdateChatLog();

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
	UpdateProgressLog();
	UpdateChatLog();
	UpdateScoreIcons();
}

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		NewWidget();
		InitWidget();
	}
	bShowMouseCursor = true;
	
}
