// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AOTCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API UAOTCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UAOTCharacterControlData();

public:
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovment)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseComtrollerDesireedRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingCotext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInhertiYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInhertiRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
