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

	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	TargetWidget->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UUserWidget> TargetWidgetRef(TEXT("/Game/Player/UI/WBP_TargetWidget.WBP_TargetWidget_C"));
	if (TargetWidgetRef.Class)
	{
		TargetWidget->SetWidgetClass(TargetWidgetRef.Class);
		TargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
		TargetWidget->SetDrawSize(FVector2D(100.0f, 60.0f));
		TargetWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAOTCharacterPlayer::CheckAimRay, 0.01f, true);
}

void AAOTCharacterPlayer::SetCameraComponent(UCameraComponent* CameraComponent)
{
	Camera = CameraComponent;
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

//	//start와 end 가 중요
//	FVector _Location;
//	FRotator _Rotation;
//	FHitResult _HitOut;
//
//	//카메라가 보고 있는 위치
//	GetController()->GetPlayerViewPoint(_Location, _Rotation);
//	FVector _Start = _Location;
//	int ViewDis = 2000;
//	FVector _End = _Location + (_Rotation.Vector() * ViewDis); // 2000은 무작위 거리
//	FCollisionQueryParams _traceParams; //trace의 params들
//	bool bHit = GetWorld()->LineTraceSingleByChannel(_HitOut, _Start, _End, GAMETRACECHANNEL, _traceParams); // 충돌 결과, 시작점, 결과, 콜리전채널, params(안넣으면 디폴트)
//
//#if ENABLE_DRAW_DEBUG
//	
//	DrawDebugLine(GetWorld(), _Start, _End, FColor::Green, false, 2.0f);
//
//#endif

	if (bHit)
	{

		UE_LOG(LogTemp, Log, TEXT("Hit"));

		const AActor* Actor = Cast<AActor>(HitResult.GetActor());
		if (Actor)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit ActorName: %s"), *Actor->GetName());
			AGiantCollisionSocket* CollisionSocket = Cast<AGiantCollisionSocket>(HitResult.GetActor());

			if (CollisionSocket)
			{
				if (CollisionSocket->bIsValid)
				{
					UE_LOG(LogTemp, Log, TEXT("Hit SocketName: %s"), *CollisionSocket->GetName());
					FVector SocketLocation = HitResult.GetActor()->GetActorLocation();
					ShowObjectLocationOnUI(SocketLocation);
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("It's not GiantSocket"));
				TargetWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}



void AAOTCharacterPlayer::ShowObjectLocationOnUI(FVector SocketLocation)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		FVector2D ScreenPos;
		bool bIsOnScreen = PlayerController->ProjectWorldLocationToScreen(SocketLocation, ScreenPos);

		if (bIsOnScreen)
		{
			UpdateUIPosition(ScreenPos);
		}
		else
		{
			TargetWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AAOTCharacterPlayer::UpdateUIPosition(FVector2D ScreenPosition)
{
	if (TargetWidget)
	{
		//TargetWidget->SetPositionInViewport(ScreenPosition);
		TargetWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::Visible);
		TargetWidget->GetUserWidgetObject()->SetPositionInViewport(ScreenPosition);
	}
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
