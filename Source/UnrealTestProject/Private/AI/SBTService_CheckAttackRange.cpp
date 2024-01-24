// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();

	if (ensure(blackboardComp))
	{
		AActor* targetActor = Cast<AActor> (blackboardComp->GetValueAsObject("TargetActor"));
		if (targetActor)
		{
			AAIController* MyController = OwnerComp.GetAIOwner();

			if (ensure(MyController))
			{
				APawn* AiPawn = MyController->GetPawn();

				if (ensure(AiPawn))
				{
					float DistanceTo = FVector::Distance(targetActor->GetActorLocation(), AiPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.0f;

					bool bHasLOS = false;
					if (bWithinRange) 
					{
						bHasLOS = MyController->LineOfSightTo(targetActor);
					}

					blackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}


			}
		}
	}
}
