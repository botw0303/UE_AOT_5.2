// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AOTPlayerController.h"

void AAOTPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
