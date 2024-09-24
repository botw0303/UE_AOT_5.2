// Fill out your copyright notice in the Description page of Project Settings.


#include "Giant/GiantAIController.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/BlackboardComponent.h>

AGiantAIController::AGiantAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAssetRef(
		TEXT("/Game/Giant/AI/GiantBlackBoard.GiantBlackBoard")
	);
	if (BlackboardAssetRef.Object != nullptr)
	{
		BlackboardAsset = BlackboardAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeAssetRef(
		TEXT("/Game/Giant/AI/GiantBehaviourTree.GiantBehaviourTree")
	);
	if (BehaviorTreeAssetRef.Object != nullptr)
	{
		BehaviorTreeAsset = BehaviorTreeAssetRef.Object;
	}
}

void AGiantAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BlackboardAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BehaviorTreeAsset);
		ensure(RunResult);
	}
}

void AGiantAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AGiantAIController::MoveToTarget(FVector TargetLocation)
{
	MoveToLocation(TargetLocation, 100.f);
}

void AGiantAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}
