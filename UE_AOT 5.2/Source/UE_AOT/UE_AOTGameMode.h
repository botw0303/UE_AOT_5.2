// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <Giant/GiantCharacter.h>
#include "UE_AOTGameMode.generated.h"

UCLASS(minimalapi)
class AUE_AOTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE_AOTGameMode();

protected:
	virtual void BeginPlay() override;

	void CheckTotalGiantCount();
	void SpawnGiant();
	bool IsLocationValid(const FVector& Location, float Radius, AActor* IgnoreActor);

protected:
	FTimerHandle TimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "SpawnGiant")
	TSubclassOf<AActor> GiantActor;
	TArray<AActor*> Giants;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnGiant")
	int MaxGiantCnt;
	bool bIsGiantCntMax;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnGiant")
	TArray<FVector> DefaultSpawnLocations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnGiant")
	int LocationIdx = 0;
};



