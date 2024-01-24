// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp_HealthPotion.h"
#include "SAttributesComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ASPowerUp_HealthPotion::ASPowerUp_HealthPotion()
{
	CreditCost = 50;
}

void ASPowerUp_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	//if (!ensure(InstanceSubobjectTemplates))
	//{
	//	return;
	//}

	USAttributesComponent* AttributeComp = Cast<USAttributesComponent>(InstigatorPawn->GetComponentByClass(USAttributesComponent::StaticClass()));

	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				HideAndCooldownPowerup();
			}
		}
	}
}

FText ASPowerUp_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{

	USAttributesComponent* AttributeComp = USAttributesComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPortion_FullHealthWarning", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPortion_InteractMessage", "Cost :{0} Credits. Rstores health to max"), CreditCost);
}


#undef LOCTEXT_NAMESPACE 