// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AOTTargetWidget.h"

UAOTTargetWidget::UAOTTargetWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UAOTTargetWidget::WidgetOnOff(bool OnOff)
{
	if (OnOff)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}
