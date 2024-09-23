// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_MoveToPatrolPos.h"
#include "AI/AIDefine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToPatrolPos::UBTTask_MoveToPatrolPos()
{
	NodeName="MoveToPatrolPos";
}

EBTNodeResult::Type UBTTask_MoveToPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(AIController)
	{
		CachedOwnerComp = &OwnerComp;
		CachedPathFollowingComponent = AIController->GetPathFollowingComponent();

		if(CachedPathFollowingComponent)
		{
			CachedPathFollowingComponent->OnRequestFinished.RemoveAll(this);
		}
		
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
		FVector PatrolPos = BlackboardComp->GetValueAsVector(BBKEY_PATROLPOS);

		EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(PatrolPos, AcceptanceRadius);

		if (MoveResult == EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTTask_MoveToPatrolPos::OnMoveCompleted);
			return EBTNodeResult::InProgress; 
		}
		else
		{
			return EBTNodeResult::Failed; 
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_MoveToPatrolPos::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) const
{
	if(Result.Code == EPathFollowingResult::Success)
	{
		if(CachedOwnerComp)
		{
			UE_LOG(LogTemp, Log, TEXT("Success"));
			CachedOwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if(CachedOwnerComp)
		{
			CachedOwnerComp->OnTaskFinished(this, EBTNodeResult::Failed);
		}
	}
 }

void UBTTask_MoveToPatrolPos::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	if (CachedPathFollowingComponent)
	{
		CachedPathFollowingComponent->OnRequestFinished.RemoveAll(this);
	}
}
