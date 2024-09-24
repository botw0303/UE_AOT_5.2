// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AOTTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API UAOTTargetWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UAOTTargetWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void WidgetOnOff(bool OnOff);
};
