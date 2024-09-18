// Fill out your copyright notice in the Description page of Project Settings.


#include "Giant/Animation/GiantAnimInstance.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include  "Giant/GiantCharacter.h"

UGiantAnimInstance::UGiantAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UGiantAnimInstance::BeginAttack()
{
	bIsAttack = true;
}

void UGiantAnimInstance::AnimNotify_ReleaseAttack()
{
	UE_LOG(LogTemp, Log, TEXT("ReleaseAttack"));
	bIsAttack = false;
	AGiantCharacter* GiantCharacter = Cast<AGiantCharacter>(Owner);
	GiantCharacter->ReleaseAttack();
}

void UGiantAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UGiantAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();	
		bIsIdle = GroundSpeed < MovingThreshould;
	}
}
