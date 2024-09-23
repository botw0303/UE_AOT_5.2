// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GiantAIController.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API AGiantAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGiantAIController();

public:
	void RunAI();
	void StopAI();

public:
	void MoveToTarget(FVector TargetLocation);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BlackboardAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BehaviorTreeAsset;
	
};
