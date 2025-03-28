// Fill out your copyright notice in the Description page of Project Settings.


#include "NCHUDWidget.h"

#include "NCMessageBoxWidget.h"

void UNCHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Blueprints 위젯 연결 체크
	ensure(EditableTextBoxWidget);
	ensure(MessageBoxWidget);
	
}

