// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GiantAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API UGiantAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UGiantAnimInstance();

public:
	void BeginAttack();

	UFUNCTION()
	void AnimNotify_ReleaseAttack();

public:
	void SetAbleStand(bool value);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Giant)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Giant)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	uint8 bIsAttack : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	uint8 bAbleStand : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Giant)
	float MovingThreshould;

};
