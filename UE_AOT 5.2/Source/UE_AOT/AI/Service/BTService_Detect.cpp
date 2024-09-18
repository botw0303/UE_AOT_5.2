// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_Detect.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Interface/BasicAIInterface.h"
#include <DrawDebugHelpers.h>
#include "AI/AIDefine.h"
#include "Collision/CollisionDefine.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DataSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DataSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	IBasicAIInterface* AIInterface = Cast<IBasicAIInterface>(ControllingPawn);
	if (AIInterface == nullptr)
	{
		return;
	}

	float DetectRange = AIInterface->GetDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		GAMETRACECHANNEL,
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRange, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, Center, Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRange, 16, FColor::Red, false, 0.2f);
}
