// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"
#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include <DrawDebugHelpers.h>
#include "../UnrealTestProject.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("start"));

	//LogOnScreen(this, "Started: " + ActionName.ToString(), FColor::Green);

	USActionComponent* comp = GetOwningComponent();

	comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Institgator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{

	//UE_LOG(LogTemp, Log, TEXT("stop"));

	//LogOnScreen(this, "Stopped: "+ ActionName.ToString(), FColor::White);

	//ensureAlways(bIsRunning);
	USActionComponent* comp = GetOwningComponent();

	comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Institgator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning()) {
		return false;
	}

	USActionComponent* comp = GetOwningComponent();

	if (comp->ActiveGameplayTags.HasAny(BlocksTags))
	{
		return false;
	}

	return true;
}


UWorld* USAction::GetWorld() const
{
	//Super::GetWorld();

	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());

	//return Actor->GetComponentByClass(USActionComponent::StaticClass());


	//return Cast<USActionComponent>(GetOuter());

	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Institgator);
	}
	else
	{
		StopAction(RepData.Institgator);
	}
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
	DOREPLIFETIME(USAction, TimeStarted);
}
