// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include <Kismet/GameplayStatics.h>
#include "BehaviorTree/BlackboardComponent.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviourTree);

	if (ensureMsgf(BehaviourTree, TEXT("Behaviour Tree is nullptr! please assign behaviourtree in AI controller")))
	{
		RunBehaviorTree(BehaviourTree);
	}

	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	//}
}
