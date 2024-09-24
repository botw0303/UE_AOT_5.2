// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_AOTGameMode.h"
#include "UE_AOTCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include <Collision/CollisionDefine.h>

AUE_AOTGameMode::AUE_AOTGameMode()
{

}

void AUE_AOTGameMode::BeginPlay()
{
	// 5마리 미리 스폰
	for (int i = 0; i < 5; ++i)
	{
		SpawnGiant();
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AUE_AOTGameMode::CheckTotalGiantCount, 60.0f, true);
}

void AUE_AOTGameMode::CheckTotalGiantCount()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGiantCharacter::StaticClass(), Giants);
	if (Giants.Num() >= MaxGiantCnt)
	{
		return;
	}

	SpawnGiant();
}

void AUE_AOTGameMode::SpawnGiant()
{
	if (GiantActor)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (NavSystem)
		{
			FNavLocation SpawnPos;

			FVector SpawnLocation = FVector(0, 0, 0);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(GiantActor, SpawnLocation, SpawnRotation);

			if (SpawnedActor)
			{
				float RandomScaleValue = FMath::FRandRange(10.0f, 15.0f);

				SpawnedActor->SetActorRelativeScale3D(FVector(RandomScaleValue));

				SpawnedActor->SetActorLocation(DefaultSpawnLocations[LocationIdx % DefaultSpawnLocations.Num()]);
				LocationIdx++;
			}
		}
	}
}

bool AUE_AOTGameMode::IsLocationValid(const FVector& Location, float Radius, AActor* IgnoreActor)
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);

	//CollisionQueryParam.AddIgnoredActor(IgnoreActor);

	bool bHasOverlaps = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Location,
		FQuat::Identity,
		GAMETRACECHANNEL,
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParam
	);

	return !bHasOverlaps || OverlapResults.Num() == 0;
}




