// Fill out your copyright notice in the Description page of Project Settings.


#include "AOTCharacterPlayer.h"
#include "Player/AOTCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Collision/CollisionDefine.h>
#include "Physics/AOTCollision.h"
#include "Giant/GiantCharacter.h"
#include "NiagaraSystem.h"
#include <NiagaraFunctionLibrary.h>
#include <NiagaraComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <Giant/CollisionSocket/GiantCollisionSocket.h>

AAOTCharacterPlayer::AAOTCharacterPlayer()
{
	SetActorTickEnabled(true);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AttackEffectNiagaraRef(
		TEXT("/Game/Effect/Slash/SlashNiagara.SlashNiagara")
	);
	if(AttackEffectNiagaraRef.Object)
	{
		AttackEffectNiagara = AttackEffectNiagaraRef.Object;
	}
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
}

void AAOTCharacterPlayer::StopStraightBoost()
{
	bIsStraightBoosting = false;

	LaunchCharacter(FVector(0, 0, 0), true, true);
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

				SpawnNiagaraSystem();
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
		float Distance = FVector::Dist(GetActorLocation(), TargetVec);
		if (Distance <= Threshould || !bIsAnchoredToGiant)
		{
			UE_LOG(LogTemp, Log, TEXT("Stop Straight Boosting"));
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

void AAOTCharacterPlayer::SpawnNiagaraSystem()
{
	if(AttackEffectNiagara)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			AttackEffectNiagara,
			GetActorLocation(),
			GetActorRotation()
		);

		if(NiagaraComponent)
		{
			NiagaraComponent->OnSystemFinished.AddDynamic(this, &AAOTCharacterPlayer::OnNiagaraSystemFinished);
		}
	}
}

void AAOTCharacterPlayer::OnNiagaraSystemFinished(UNiagaraComponent* System)
{
	if(System)
	{
		System->DestroyComponent();
	}

	NiagaraComponent = nullptr;
}