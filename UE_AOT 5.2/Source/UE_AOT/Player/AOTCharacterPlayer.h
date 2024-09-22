// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AOTCharacterBase.h"
#include "InputActionValue.h"
#include "AOTCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API AAOTCharacterPlayer : public AAOTCharacterBase
{
	GENERATED_BODY()
	
public:
	AAOTCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	bool isAnchored;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool CheckAnchoredTargetIsGiant();	// ��Ŀ�� ������ ����� �������� Ȯ���ϰ� bool ���� ��ȯ
	void MoveActorToTargetStraight();	// ����ڸ� ��Ŀ�� ������ ��ġ�� ���� �������� �̵�
	void Attack();						// ����

	// Character Control Section
protected:
	//virtual void SetCharacterContrlData(const UAOTCharacterControlData* CharacterControlData) override;

	// Camera Section
protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	//TObjectPtr<class USpringArmComponent> CameraBoom;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	//TObjectPtr<class UCameraComponent> FollowCamera;

	// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
};
