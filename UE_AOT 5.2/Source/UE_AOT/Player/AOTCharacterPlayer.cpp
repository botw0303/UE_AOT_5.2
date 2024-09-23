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

AAOTCharacterPlayer::AAOTCharacterPlayer()
{
	SetActorTickEnabled(true);
}

void AAOTCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	//APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//{
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//}
}

void AAOTCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAOTCharacterPlayer::Attack);
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
			if(Actor == nullptr)
			{
				continue;
			}
			
			const float Distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
			if (Distance > AttackRange)
			{
				continue;
			}
			
			if (AGiantCollisionSocket* CollisionSocket = Cast<AGiantCollisionSocket>(OverlapResult.GetActor()))
			{
				if(!CollisionSocket->bIsValid)
				{
					continue;
				}
				
				CollisionSocket->OnHit();
				return;
			}
		}
	}
}

void AAOTCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if ENABLE_DRAW_DEBUG

	FColor DrawColor = FColor::Yellow;
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 16, DrawColor);

#endif

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

