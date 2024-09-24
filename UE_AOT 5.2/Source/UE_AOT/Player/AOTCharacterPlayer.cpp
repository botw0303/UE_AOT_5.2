// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AOTCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Collision/CollisionDefine.h>
#include "Physics/AOTCollision.h"
#include "Giant/GiantCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Giant/CollisionSocket/GiantCollisionSocket.h>
#include <Kismet/GameplayStatics.h>
#include "Components/WidgetComponent.h"

AAOTCharacterPlayer::AAOTCharacterPlayer()
{
	SetActorTickEnabled(true);
}

void AAOTCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (TargetWidgetBPActor)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TargetWidgetBPActor, SpawnLocation, SpawnRotation);

		TargetWidgetActor = SpawnedActor;
	}
}

void AAOTCharacterPlayer::SetCameraComponent(UCameraComponent* CameraComponent)
{
	Camera = CameraComponent;
}

void AAOTCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAOTCharacterPlayer::CheckAnchoredTargetIsGiant(FVector Start)
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Detect), false, this);

	const float DetectRadius = 30.0f;

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Start,
		FQuat::Identity,
		GAMETRACECHANNEL,
		FCollisionShape::MakeSphere(DetectRadius),
		Params
	);

	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn)
			{
				AGiantCharacter* GiantComponent = Cast<	AGiantCharacter>(Pawn);
				if (GiantComponent)
				{
					bIsAnchoredToGiant = true;
					UE_LOG(LogTemp, Log, TEXT("Is Giant"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Is Not Giant"));
			}
		}
	}
}

void AAOTCharacterPlayer::StraightBoost()
{
	LaunchCharacter(BoostDirection * BoostSpeed, true, true);
}

void AAOTCharacterPlayer::StartStraightBoost(FVector Direction, FVector TargetVector)
{
	BoostDirection = Direction;
	TargetVec = TargetVector;

	bIsStraightBoosting = true;
	bCanAttack = true;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAOTCharacterPlayer::StopStraightBoost, CalcTimeBasedOnDistance(TargetVector), false);
}

void AAOTCharacterPlayer::StopStraightBoost()
{
	bCanAttack = false;
	bIsStraightBoosting = false;

	LaunchCharacter(BoostDirection * BoostSpeed / 3, true, true);
}

void AAOTCharacterPlayer::Attack()
{
	// GiantCollisionSocket <- ���⿡ �ִ� OnHit �̰͸� �����Ű�� ��
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		GAMETRACECHANNEL,
		FCollisionShape::MakeSphere(AttackRange),
		CollisionQueryParam
	);

	UE_LOG(LogTemp, Log, TEXT("Try Attack"));

	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			const AActor* Actor = Cast<AActor>(OverlapResult.GetActor());
			if (Actor == nullptr)
			{
				continue;
			}

			UE_LOG(LogTemp, Log, TEXT("Try Attack2"));

			const float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
			if (Distance > AttackRange)
			{
				continue;
			}

			UE_LOG(LogTemp, Log, TEXT("Try Attack3"));

			if (AGiantCollisionSocket* CollisionSocket = Cast<AGiantCollisionSocket>(OverlapResult.GetActor()))
			{
				if (!CollisionSocket->bIsValid)
				{
					continue;
				}

				UE_LOG(LogTemp, Log, TEXT("Try Attack4"));

				CollisionSocket->OnHit();
				return;
			}
		}
	}
}

void AAOTCharacterPlayer::FindNearbyGiants()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, this);

	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		GAMETRACECHANNEL,
		FCollisionShape::MakeSphere(AutoLockDetectRange),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			const AActor* Actor = Cast<AActor>(OverlapResult.GetActor());
			if (Actor == nullptr)
			{
				continue;
			}

			if (AGiantCollisionSocket* CollisionSocket = Cast<AGiantCollisionSocket>(OverlapResult.GetActor()))
			{
				if (!CollisionSocket->bIsValid)
				{
					continue;
				}

				CheckAimRay();
			}
		}
	}
}

void AAOTCharacterPlayer::CheckAimRay()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (Camera->GetForwardVector() * AutoLockDetectRange);
	FCollisionQueryParams TraceParams(FName(TEXT("TraceTag")), false, this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		GAMETRACECHANNEL,
		TraceParams
	);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = FColor::Purple;
	DrawDebugLine(GetWorld(), Start, End, DrawColor);

#endif

	if (bHit)
	{
		const AActor* Actor = Cast<AActor>(HitResult.GetActor());
		if (Actor)
		{
			AGiantCollisionSocket* CollisionSocket = Cast<AGiantCollisionSocket>(HitResult.GetActor());

			if (CollisionSocket)
			{
				if (CollisionSocket->bIsValid)
				{
					FVector SocketLocation = CollisionSocket->GetActorLocation();
					TargetWidgetActor->SetActorHiddenInGame(false);
					TargetWidgetActor->SetActorLocation(SocketLocation);
					bIsAutoLockingNow = true;
					LockingLocation = SocketLocation;
					return;
				}
			}
		}
	}

	bIsAutoLockingNow = false;
	TargetWidgetActor->SetActorHiddenInGame(true);
}



void AAOTCharacterPlayer::ShowObjectLocationOnUI(FVector SocketLocation)
{
	/*APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("UITEST1"));

		FVector2D ScreenPos;
		bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(SocketLocation, ScreenPos);

		if (bIsOnScreen)
		{
			UE_LOG(LogTemp, Log, TEXT("UITEST2"));

			UE_LOG(LogTemp, Log, TEXT("ScreenLocation: %s"), *ScreenPos.ToString());

			UpdateUIPosition(ScreenPos);
		}
		else
		{
			TargetWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/
}

void AAOTCharacterPlayer::UpdateUIPosition(FVector2D ScreenPosition)
{
	//if (TargetWidget)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("UITEST3"));
	//	//TargetWidget->SetPositionInViewport(ScreenPosition);
	//	TargetWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);

	//	UE_LOG(LogTemp, Log, TEXT("UILocation: %s"), *ScreenPosition.ToString());

	//	//TargetWidget->GetUserWidgetObject()->SetPositionInViewport(ScreenPosition);
	//	TargetWidget->SetWorldLocation(FVector(ScreenPosition.X, ScreenPosition.Y, 0));
	//}
}

void AAOTCharacterPlayer::AutoLockOn()
{

}

void AAOTCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = FColor::Yellow;
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 16, DrawColor);

	DrawColor = FColor::Purple;
	DrawDebugSphere(GetWorld(), GetActorLocation(), AutoLockDetectRange, 16, DrawColor);

#endif

	CheckAimRay();

	if (bIsStraightBoosting)
	{
		if (!bIsAnchoredToGiant)
		{
			UE_LOG(LogTemp, Log, TEXT("Stop Straight Boosting"));
			if (bCanAttack) Attack();
			StopStraightBoost();	// �̵� ����
		}
		else
		{
			StraightBoost(); // �̵� ����
			UE_LOG(LogTemp, Log, TEXT("Straight Boosting"));
		}
	}
}

void AAOTCharacterPlayer::SetActorTickEnabled(bool bEnabled)
{
	PrimaryActorTick.SetTickFunctionEnable(bEnabled);
}

float AAOTCharacterPlayer::CalcTimeBasedOnDistance(FVector TargetLocation)
{
	float Distance = FVector::Dist(GetActorLocation(), TargetLocation);

	float Time = (MaxTime / MaxDistance) * Distance;

	return FMath::Clamp(Time, 0.0f, MaxTime);
}







