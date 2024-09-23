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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack")
	float AttackBoostTime = 1.5f;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CheckAnchoredTargetIsGiant(FVector Start);						// 앵커가 부착된 대상이 거인인지 확인하고 bool 값을 반환

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StraightBoost();												// 사용자를 앵커가 부착된 위치를 향해 직선으로 이동 (Tick 마다 실행해서 지속 이동하도록)

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartStraightBoost(FVector Direction, FVector TargetVector);	// 앵커가 부착된 방향을 저장하고 StraightBoost가 실행되도록 설정

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopStraightBoost();											// 앵커 위치에 도달 혹은 이동 중 스페이스바 입력이 끊기면 이동 중지 처리

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();														// 공격

/*	UFUNCTION(BlueprintCallable, Category = "Auto LockOn")
	void FindNearbyGiants();*/											// 주변 거인을 탐색


	virtual void Tick(float DeltaTime) override;

	// Tick을 활성화하기 위한 플래그
	virtual void SetActorTickEnabled(bool bEnabled);
	
private:
	FTimerHandle TimerHandle;
	float LastDistance;

protected:
	FVector BoostDirection;
	FVector TargetVec;

	const float MaxDistance = 10000.0f;  // 최대 거리
	const float MaxTime = 4.0f;          // 최대 시간

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
};
