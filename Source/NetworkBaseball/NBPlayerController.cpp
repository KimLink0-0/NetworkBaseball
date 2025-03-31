// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

#include "NBGameMode.h"
#include "NBGameProgressWidget.h"
#include "NBHUDWidget.h"
#include "NBPlayerState.h"
#include "NBSBOScreenWidget.h"
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

void ANBPlayerController::ServerRPCRequestSetUserPitch_Implementation(const FString& Message)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	SetUserPitch(Message);

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::ServerRPCRequestJudgePlay_Implementation(const FName& UserName)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))

	RequestJudgePlay(UserName);

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::ClientRPCRequestUpdateWidget_Implementation()
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))

	RequestUpdateScreen();
	RequestUpdateProgress();
	
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::RequestJudgePlay(const FName& UserName)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	auto* GameMode = Cast<ANBGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		auto* NBPlayerState = GetPlayerState<ANBPlayerState>();
		if (NBPlayerState)
		{
			const FString PlayResult = GameMode->JudgePlayResult(UserName);
			NBPlayerState->SetPlayResult(PlayResult);
		}
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::RequestUpdateScreen() const
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	auto* SBOScreen = Cast<UNBSBOScreenWidget>(GetHUDWidgetInstance()->SBOScreenWidget);
	if (SBOScreen)
	{
		SBOScreen->UpdateScreen();
	}

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::RequestUpdateProgress() const
{
	auto* Progress = Cast<UNBGameProgressWidget>(GetHUDWidgetInstance()->ProgressWidget);
	if (Progress)
	{
		Progress->UpdateDisplayInfo();
	}
}

void ANBPlayerController::SendMessageToNetwork(const FString& MessageToSend)
{
	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("Begin"))
	
	ServerRPCRequestSetUserPitch(MessageToSend);

	NB_LOG(LogBaseBall, Log, TEXT("%s"), TEXT("End"))
}

void ANBPlayerController::SetUserPitch(const FString& Message)
{
	auto* NBPlayerState = GetPlayerState<ANBPlayerState>();
	if (NBPlayerState)
	{
		NBPlayerState->SetUserPitchString(Message);
	}
}

void ANBPlayerController::InitWidget()
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

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		InitWidget();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&ANBPlayerController::ClientRPCRequestUpdateWidget,
			0.2f,
			false
		);
		
	}
	bShowMouseCursor = true;
	
}
