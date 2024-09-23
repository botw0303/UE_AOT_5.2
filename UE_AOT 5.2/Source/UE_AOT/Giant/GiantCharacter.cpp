// Fill out your copyright notice in the Description page of Project Settings.


#include "GiantCharacter.h"
#include "GiantAIController.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Collision/CollisionDefine.h"
#include "Giant/Animation/GiantAnimInstance.h"
#include "Giant/CollisionSocket/GiantCollisionSocket.h"

// Sets default values
AGiantCharacter::AGiantCharacter()
{
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -140.f), FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

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

	// Collision Socket Setting
	static ConstructorHelpers::FClassFinder<AGiantCollisionSocket> CollisionSocketClassRef(
		TEXT("/Game/Giant/Blueprint/BP_GiantCollisionSocket.BP_GiantCollisionSocket_C")
	);
	
	if(CollisionSocketClassRef.Class)
	{
		GiantCollisionSocketClass = CollisionSocketClassRef.Class;
	}

	// Find Separate Actor
	static ConstructorHelpers::FClassFinder<AActor> Forearm_LRef(TEXT("/Game/Giant/Blueprint/SeparateMeshActor/BP_Forearm_L.BP_Forearm_L_C"));
	static ConstructorHelpers::FClassFinder<AActor> Forearm_RRef(TEXT("/Game/Giant/Blueprint/SeparateMeshActor/BP_Forearm_R.BP_Forearm_R_C"));
	static ConstructorHelpers::FClassFinder<AActor> Shin_RRef(TEXT("/Game/Giant/Blueprint/SeparateMeshActor/BP_Shin_L.BP_Shin_L_C"));
	static ConstructorHelpers::FClassFinder<AActor> Shin_LRef(TEXT("/Game/Giant/Blueprint/SeparateMeshActor/BP_Shin_R.BP_Shin_R_C"));

	if(Forearm_LRef.Class)
	{
		SeparateActor.Add(EGiantSocketType::Forearm_LSocket, Forearm_LRef.Class);
	}
	if(Forearm_RRef.Class)
	{
		SeparateActor.Add(EGiantSocketType::Forearm_RSocket, Forearm_RRef.Class);
	}
	if(Shin_LRef.Class)
	{
		SeparateActor.Add(EGiantSocketType::Shin_LSocket, Shin_LRef.Class);
	}
	if(Shin_RRef.Class)
	{
		SeparateActor.Add(EGiantSocketType::Shin_RSocket, Shin_RRef.Class);
	}
}

// Called when the game starts or when spawned
void AGiantCharacter::BeginPlay()
{
	Super::BeginPlay();

	bAbleAttack = true;
	
	for(int i = 0; i < EGiantSocketTypeCnt; i++)
	{
		EGiantSocketType SocketType = static_cast<EGiantSocketType>(i);
		FName SocketName;

		switch (SocketType) {
		case EGiantSocketType::HeadSocket:
			SocketName = TEXT("HeadSocket");
			break;
		case EGiantSocketType::Forearm_LSocket:
			SocketName = TEXT("Forearm_LSocket");
			break;
		case EGiantSocketType::Forearm_RSocket:
			SocketName = TEXT("Forearm_RSocket");
			break;
		case EGiantSocketType::Shin_LSocket:
			SocketName = TEXT("Shin_LSocket");
			break;
		case EGiantSocketType::Shin_RSocket:
			SocketName = TEXT("Shin_RSocket");
			break;
		default:
			SocketName = TEXT("Invalid");
			break;
		}
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AGiantCollisionSocket* CollisionSocket = GetWorld()->SpawnActor<AGiantCollisionSocket>(GiantCollisionSocketClass, SpawnParams);
		CollisionSocket->SocketType = SocketType;
		CollisionSocket->OwnerGiant = this;

		CollisionSocket->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}

	// OnDamage(EGiantSocketType::Forearm_LSocket);
	// OnDamage(EGiantSocketType::Forearm_RSocket);
	// OnDamage(EGiantSocketType::Shin_LSocket);
	// OnDamage(EGiantSocketType::Shin_RSocket);
}

void AGiantCharacter::OnDamage(EGiantSocketType DamagedSocketType)
{
	if(DamagedSocketType == EGiantSocketType::HeadSocket)
	{
		Die();
		return;
	}
	
	FName SocketName;

	switch (DamagedSocketType) {
	case EGiantSocketType::Forearm_LSocket:
		SocketName = TEXT("Forearm_LSocket");
		break;
	case EGiantSocketType::Forearm_RSocket:
		SocketName = TEXT("Forearm_RSocket");
		break;
	case EGiantSocketType::Shin_LSocket:
		SocketName = TEXT("Shin_LSocket");
		break;
	case EGiantSocketType::Shin_RSocket:
		SocketName = TEXT("Shin_RSocket");
		break;
	default:
		SocketName = TEXT("Invalid");
		break;
	}
	
	FName SocketBoneName = GetMesh()->GetSocketBoneName(SocketName);

	GetMesh()->HideBoneByName(SocketBoneName, PBO_None);

	FTransform BoneTransform = GetMesh()->GetBoneTransform(GetMesh()->GetBoneIndex(SocketBoneName));
	AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(SeparateActor[DamagedSocketType], BoneTransform);
	SpawnActor->SetActorScale3D(GetTransform().GetScale3D());
	SpawnActor->SetLifeSpan(5.f);

	switch (DamagedSocketType)
	{
	case EGiantSocketType::Forearm_LSocket:
	case EGiantSocketType::Forearm_RSocket:
		{
			bAbleAttack = false;
		}
		break;
	case EGiantSocketType::Shin_LSocket:
	case EGiantSocketType::Shin_RSocket:
		{
			UGiantAnimInstance* GiantAnimInstance = Cast<UGiantAnimInstance>(GetAnimInstance());
			GiantAnimInstance->SetAbleStand(false);
			AGiantAIController* GiantAIController = Cast<AGiantAIController>(GetController());
			GiantAIController->StopAI();
		}
		break;
	}
}

void AGiantCharacter::Die()
{
	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
	GetMesh()->Stop();
	GetMesh()->SetSimulatePhysics(true);
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
	return 10000.f;
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

bool AGiantCharacter::GetAbleAttack()
{
	return bAbleAttack;
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
