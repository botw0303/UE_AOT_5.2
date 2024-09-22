// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantCharacter.h"
#include "GiantAIController.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Collision/CollisionDefine.h"
#include "Giant/Animation/GiantAnimInstance.h"

// Sets default values
AGiantCharacter::AGiantCharacter()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -140.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 100.f;

	AIControllerClass = AGiantAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GiantMeshRef(
		TEXT("/Game/Giant/Model/GiantDummy.GiantDummy")
	);
	if (GiantMeshRef.Object != nullptr)
	{
		GetMesh()->SetSkeletalMesh(GiantMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassRef(
		TEXT("/Game/Giant/Animation/GiantAnimBlueprint.GiantAnimBlueprint_C")
	);
	if (AnimClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimClassRef.Class);
	}
}

// Called when the game starts or when spawned
void AGiantCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGiantCharacter::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.f;
	const float AttackRadius = 50.f;
	const float AttackDamage = 30.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRadius;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, GAMETRACECHANNEL, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (HitDetected)
	{
		UGiantAnimInstance* AnimInstnace = Cast<UGiantAnimInstance>(GetMesh()->GetAnimInstance());
		AnimInstnace->BeginAttack();
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), HitDetected ? TEXT("true") : TEXT("false"));

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRadius * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float AGiantCharacter::GetPatrolRadius()
{
	return 800.f;
}

float AGiantCharacter::GetDetectRange()
{
	return 400.0f;
}

float AGiantCharacter::GetAttackRange()
{
	return 200.0f;
}

float AGiantCharacter::GetTurnSpeed()
{
	return 1.f;
}

UAnimInstance* AGiantCharacter::GetAnimInstance()
{
	return GetMesh()->GetAnimInstance();
}

void AGiantCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AGiantCharacter::AIAttack()
{
	UE_LOG(LogTemp, Log, TEXT("Attack"));
	UGiantAnimInstance* AnimInstance = Cast<UGiantAnimInstance>(GetAnimInstance());
	AnimInstance->BeginAttack();
	AttackHitCheck();
}

void AGiantCharacter::ReleaseAttack()
{
	OnAttackFinished.ExecuteIfBound();
}
