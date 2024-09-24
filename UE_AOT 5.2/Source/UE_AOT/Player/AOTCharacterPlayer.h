// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AOTCharacterBase.h"
#include "InputActionValue.h"
#include <Giant/CollisionSocket/GiantCollisionSocket.h>
#include "Camera/CameraComponent.h"
#include "AOTCharacterPlayer.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	bool bIsAnchoredToGiant;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	float BoostSpeed = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	float Threshould = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	float AttackRange = 60.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	bool bCanAttack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoLockOn")
	TArray<AGiantCollisionSocket*> GiantColSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoLockOn")
	float AutoLockDetectRange = 5000.0f;

	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "AutoLockOn")
	TSubclassOf<AActor> TargetWidgetBPActor; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AutoLockOn")
	bool bIsAutoLockingNow = false;

	UPROPERTY(BlueprintReadOnly, Category = "AutoLockOn")
	FVector LockingLocation;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CheckAnchoredTargetIsGiant(FVector Start);						// ��Ŀ�� ������ ����� �������� Ȯ���ϰ� bool ���� ��ȯ

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StraightBoost();												// ����ڸ� ��Ŀ�� ������ ��ġ�� ���� �������� �̵� (Tick ���� �����ؼ� ���� �̵��ϵ���)

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartStraightBoost(FVector Direction, FVector TargetVector);	// ��Ŀ�� ������ ������ �����ϰ� StraightBoost�� ����ǵ��� ����

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopStraightBoost();											// ��Ŀ ��ġ�� ���� Ȥ�� �̵� �� �����̽��� �Է��� ����� �̵� ���� ó��

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();														// ����

	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void FindNearbyGiants();											// �ֺ� GiantCollisionSocket�� Ž��

	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void CheckAimRay();

	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void ShowObjectLocationOnUI(FVector SocketLocation);

	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void UpdateUIPosition(FVector2D ScreenPosition);

	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void AutoLockOn();													// ������ ����Ű�� ���� ������ ���� �� �ϳ��̸� �ڵ� �Ͽ� ���� �̳��� ��� ��Ŀ�� �ڵ����� ������ ���� ���� �߾ӿ� �������� ��


	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraComponent(UCameraComponent* CameraComponent);

	virtual void Tick(float DeltaTime) override;

	// Tick�� Ȱ��ȭ�ϱ� ���� �÷���
	virtual void SetActorTickEnabled(bool bEnabled);

public:
	void SpawnNiagaraSystem();

	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* System);
	
private:
	FTimerHandle TimerHandle;
	float LastDistance;

	AActor* TargetWidgetActor;

protected:
	FVector BoostDirection;
	FVector TargetVec;

	const float MaxDistance = 10000.0f;  // �ִ� �Ÿ�
	const float MaxTime = 4.0f;          // �ִ� �ð�


protected:
	float CalcTimeBasedOnDistance(FVector TargetLocation);

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	bool bIsStraightBoosting;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere, Category = "Niagara")
	UNiagaraSystem* AttackEffectNiagara;
	
};
