// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_GiantAttack.h"
#include "AIController.h"
#include "Interface/BasicAIInterface.h"
#include "Giant/Animation/GiantAnimInstance.h"

UBTTask_GiantAttack::UBTTask_GiantAttack()
{

}

EBTNodeResult::Type UBTTask_GiantAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
    if (ControllingPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    IBasicAIInterface* AIPawn = Cast<IBasicAIInterface>(ControllingPawn);
    if (AIPawn == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    FAICharacterAttackFinished OnAttackFinished;
    OnAttackFinished.BindLambda(
        [&]() {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );

    AIPawn->SetAIAttackDelegate(OnAttackFinished);
    AIPawn->AIAttack();
    return EBTNodeResult::InProgress;
}
