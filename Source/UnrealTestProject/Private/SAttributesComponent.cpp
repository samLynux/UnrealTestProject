// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributesComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float>CVarDamageMultiplier(TEXT("su.multiply"), 1.0f, TEXT("enable spawning of bots via timer"));




// Sets default values for this component's properties
USAttributesComponent::USAttributesComponent()
{

	HealthMax = 100;
	Health = HealthMax;

	SetIsReplicatedByDefault(true);
}



bool USAttributesComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
	
}

bool USAttributesComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributesComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributesComponent::GetHealth() const
{
	return Health;
}

float USAttributesComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributesComponent::ApplyHealthChange(AActor* InstigatorActor, float delta)
{


	if (!GetOwner()->CanBeDamaged() && delta < 0.0f)
	{
		return false;
	}

	if (delta < 0.0f)
	{
		float damageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		delta *= damageMultiplier;
	}

	float oldHealth = Health;
	float NewHealth = FMath::Clamp(Health + delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - oldHealth;


	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (ActualDelta != 0.0f)
		{
			MultiCaastHealthChanged(InstigatorActor, Health, ActualDelta);
		}
		//ded
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);



	//UE_LOG(LogTemp, Log, TEXT("%f %f %f %f"), ActualDelta, Health, oldHealth, delta);

	return ActualDelta != 0;

}


USAttributesComponent* USAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributesComponent>(FromActor->GetComponentByClass(USAttributesComponent::StaticClass()));;

	}
	return nullptr;
}

bool USAttributesComponent::IsActorAlive(AActor* Actor)
{
	USAttributesComponent* attributeComp = GetAttributes(Actor);

	if (attributeComp)
	{
		return attributeComp->IsAlive();
	}
	return false;
}

void USAttributesComponent::MultiCaastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);


}


void USAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributesComponent, Health);
	DOREPLIFETIME(USAttributesComponent, HealthMax);
	//DOREPLIFETIME_CONDITION(USAttributesComponent, HealthMax, COND_OwnerOnly);
}