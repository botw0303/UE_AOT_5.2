// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GiantAttack.generated.h"

/**
 * 
 */
UCLASS()
class UE_AOT_API UBTTask_GiantAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GiantAttack();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
