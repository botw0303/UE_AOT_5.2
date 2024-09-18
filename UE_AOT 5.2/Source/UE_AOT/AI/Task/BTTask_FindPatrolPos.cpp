// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_FindPatrolPos.h"
#include <AIController.h>
#include <NavigationSystem.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Interface/BasicAIInterface.h"
#include "AI/AIDefine.h"


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IBasicAIInterface* AIInterface = Cast<IBasicAIInterface>(ControllingPawn);
	if (AIInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector OriginPos = ControllingPawn->GetActorLocation();
	float PatrolRadius = AIInterface->GetPatrolRadius();
	FNavLocation NewPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(OriginPos, PatrolRadius, NewPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NewPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
