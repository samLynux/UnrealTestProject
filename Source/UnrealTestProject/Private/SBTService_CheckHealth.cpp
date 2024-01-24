// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckHealth.h"
#include "SAttributesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"



USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthFraction = 0.3f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AiPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AiPawn))
	{
		USAttributesComponent* attributeComp = USAttributesComponent::GetAttributes(AiPawn);
		if (ensure(attributeComp))
		{
			bool bLowHealth = (attributeComp->GetHealth() / attributeComp->GetHealthMax()) < LowHealthFraction;

			UBlackboardComponent* blackBoardComp = OwnerComp.GetBlackboardComponent();
			blackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}