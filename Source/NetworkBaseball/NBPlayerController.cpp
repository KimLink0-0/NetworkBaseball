// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

#include "NBGameMode.h"
#include "NetworkBaseball.h"

ANBPlayerController::ANBPlayerController()
{
	// static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(TEXT(""));
	// if (HUDWidget.Succeeded())
	// {
	// 	// HUDWidgetClass = HUDWidget.Class;
	// }
	//
	// HUDWidgetInstance = nullptr;

}


void ANBPlayerController::InitWidget()
{
}

void ANBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitWidget();
}
