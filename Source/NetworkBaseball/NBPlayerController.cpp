// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

#include "NBHUDWidget.h"
#include "NBSBOScreenWidget.h"

ANBPlayerController::ANBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(TEXT("/Game/Blueprints/WBP_NBHUDWidget.WBP_NBHUDWidget_c"));
	if (HUDWidget.Succeeded())
	{
		HUDWidgetClass = HUDWidget.Class;
	}
	
	HUDWidgetInstance = nullptr;

}

void ANBPlayerController::RequestUpdateScreen() const
{
	auto* SBOScreen = Cast<UNBSBOScreenWidget>(GetHUDWidgetInstance()->SBOScreenWidget);
	if (SBOScreen)
	{
		SBOScreen->UpdateScreen();
	}
}

void ANBPlayerController::SendMessageToNetwork(const FString& MessageToSend)
{
	
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
		RequestUpdateScreen();
	}
	bShowMouseCursor = true;
	
}
