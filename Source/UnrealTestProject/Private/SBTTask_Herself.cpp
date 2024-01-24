// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTTask_Herself.h"
#include "SAttributesComponent.h"
#include "AIController.h"

EBTNodeResult::Type USBTTask_Herself::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributesComponent* attributeComp = USAttributesComponent::GetAttributes(MyPawn);
	if (ensure(attributeComp))
	{
		attributeComp->ApplyHealthChange(MyPawn, attributeComp->GetHealthMax());
	}
	return EBTNodeResult::Succeeded;
}
