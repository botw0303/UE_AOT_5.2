// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AOTAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API UAOTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AnimNotify_ReleaseAttack();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	uint8 bIsAttack : 1;
	
};
