// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BasicAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBasicAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);

/**
 * 
 */
class UE_AOT_API IBasicAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetPatrolRadius() = 0;
	virtual float GetDetectRange() = 0;
	virtual float GetAttackRange() = 0;
	virtual float GetTurnSpeed() = 0;

public:
	virtual UAnimInstance* GetAnimInstance() = 0;

public:
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AIAttack() = 0;

};
