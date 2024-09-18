// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AOTGameMode.h"
#include "Player/AOTPlayerController.h"

AAOTGameMode::AAOTGameMode()
{
	// DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/Blueprint/BP_AOTCharacterPlayer.BP_AOTCharacterPlayer_C"));
	if(ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/UE_AOT.AOTCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	// PlayerControllerClass
	//PlayerControllerClass = AAOTPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UE_AOT.AOTPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
