// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPatrolPos.generated.h"

class UPathFollowingComponent;
struct FPathFollowingResult;
/**
 * 
 */
UCLASS()
class UE_AOT_API UBTTask_MoveToPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToPatrolPos();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) const;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptanceRadius = 100.f;

private:
	UBehaviorTreeComponent* CachedOwnerComp;
	UPathFollowingComponent* CachedPathFollowingComponent;
	
};
