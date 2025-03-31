// Fill out your copyright notice in the Description page of Project Settings.


#include "NBHUDWidget.h"

void UNBHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(ProgressWidget);
	ensure(ScoreWidget);
}
