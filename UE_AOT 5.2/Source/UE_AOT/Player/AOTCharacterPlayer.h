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
	float BoostSpeed = 10.0f;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CheckAnchoredTargetIsGiant(FVector Start);	// 앵커가 부착된 대상이 거인인지 확인하고 bool 값을 반환

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StraightBoost();	// 사용자를 앵커가 부착된 위치를 향해 직선으로 이동

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartStraightBoost(FVector Direction);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopStraightBoost();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();						// 공격
	
private:
	FTimerHandle TimerHandle;

protected:
	FVector BoostDirection;

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
