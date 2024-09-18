// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_AttackInRange.h"
#include "AI/AIDefine.h"
#include "Interface/BasicAIInterface.h"
#include "AIController.h"
#include <BehaviorTree/BlackboardComponent.h>

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return false;
	}

	IBasicAIInterface* AIPawn = Cast<IBasicAIInterface>(ControllingPawn);
	if (AIPawn == nullptr)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (Target == nullptr)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);
	return bResult;
}
