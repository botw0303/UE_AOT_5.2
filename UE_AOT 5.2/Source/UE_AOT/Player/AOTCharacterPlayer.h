// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/AOTCharacterBase.h"
#include "InputActionValue.h"
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

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void CheckAnchoredTargetIsGiant(FVector Start);	// ��Ŀ�� ������ ����� �������� Ȯ���ϰ� bool ���� ��ȯ

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StraightBoost();							// ����ڸ� ��Ŀ�� ������ ��ġ�� ���� �������� �̵� (Tick ���� �����ؼ� ���� �̵��ϵ���)

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StartStraightBoost(FVector Direction, FVector TargetVector);		// ��Ŀ�� ������ ������ �����ϰ� StraightBoost�� ����ǵ��� ����

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void StopStraightBoost();						// ��Ŀ ��ġ�� ���� Ȥ�� �̵� �� �����̽��� �Է��� ����� �̵� ���� ó��

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();									// ����

	virtual void Tick(float DeltaTime) override;

	// Tick�� Ȱ��ȭ�ϱ� ���� �÷���
	virtual void SetActorTickEnabled(bool bEnabled);

public:
	void SpawnNiagaraSystem();

	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* System);
	
private:
	FTimerHandle TimerHandle;

protected:
	FVector BoostDirection;
	FVector TargetVec;

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
