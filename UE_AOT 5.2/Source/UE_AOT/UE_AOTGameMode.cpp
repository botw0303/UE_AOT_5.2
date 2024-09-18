// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_AOTGameMode.h"
#include "UE_AOTCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_AOTGameMode::AUE_AOTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
