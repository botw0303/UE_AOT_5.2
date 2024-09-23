// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/BasicAIInterface.h"
#include "Giant/CollisionSocket/GiantCollisionSocket.h"
#include "GiantCharacter.generated.h"

UCLASS()
class UE_AOT_API AGiantCharacter : public ACharacter, public IBasicAIInterface
{
	GENERATED_BODY()

public:
	AGiantCharacter();

protected:
	virtual void BeginPlay() override;

public:
	void OnDamage(EGiantSocketType DamagedSocketType);
	void Die();
	
public:
	void AttackHitCheck();

public:
	virtual float GetPatrolRadius() override;
	virtual float GetDetectRange() override;
	virtual float GetAttackRange() override;
	virtual float GetTurnSpeed() override;
	virtual bool GetAbleAttack() override;

public:
	virtual UAnimInstance* GetAnimInstance() override;

public:
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AIAttack() override;

	void ReleaseAttack();

	FAICharacterAttackFinished OnAttackFinished;

public:
	UPROPERTY(VisibleAnywhere, Category = "CollisionSocket")
	TSubclassOf<AGiantCollisionSocket> GiantCollisionSocketClass;
	
	UPROPERTY(EditAnywhere, Category = "Separated Actor")
	TMap<EGiantSocketType, TSubclassOf<AActor>> SeparateActor;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	uint8 bAbleAttack : 1;
	
};
