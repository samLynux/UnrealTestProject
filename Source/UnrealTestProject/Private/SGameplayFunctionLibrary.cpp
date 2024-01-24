// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"
#include "SAttributesComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* damageCauser, AActor* targetActor, float damageAmount)
{
	USAttributesComponent* attributeComp = USAttributesComponent::GetAttributes(targetActor);
	if (attributeComp)
	{
		return attributeComp->ApplyHealthChange(damageCauser, -damageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* damageCauser, AActor* targetActor, float damageAmount, const FHitResult& hitResult)
{
	if(ApplyDamage(damageCauser, targetActor, damageAmount))
	{
		UPrimitiveComponent* hitComp = hitResult.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics(hitResult.BoneName))
		{
			FVector direction = hitResult.TraceEnd - hitResult.TraceStart;
			direction.Normalize();
			hitComp->AddImpulseAtLocation(direction * 300000.f, hitResult.ImpactPoint, hitResult.BoneName);
		}
		return true;
	}
	return false;
}
