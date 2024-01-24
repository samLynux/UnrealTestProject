// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAiCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributesComponent.h"
#include "BrainComponent.h"
#include "SWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "SActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAiCharacter::ASAiCharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");


    AttributeComp = CreateDefaultSubobject<USAttributesComponent>("AttributeComp");


    ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

   // GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
    GetMesh()->SetGenerateOverlapEvents(true);

    TimeToHitParamName = "TimeToHit";
}



void ASAiCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAiCharacter::OnPawnSeen);
    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAiCharacter::OnHealthChanged);
}

AActor* ASAiCharacter::GetTargetActor() const
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        return Cast<AActor>(AIC->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
    }
    return nullptr;
}

void ASAiCharacter::OnPawnSeen(APawn* Pawn)
{
    if (GetTargetActor() != Pawn)
    {
        SetTargetActor(Pawn);


    }
}

void ASAiCharacter::MulticastPawnSeen_Implementation()
{
    USWorldUserWidget* NewWidget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);

    if (NewWidget)
    {
        NewWidget->AttachedActor = this;

        NewWidget->AddToViewport(10);
    }
}

void ASAiCharacter::SetTargetActor(AActor* newTarget)
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC)
    {
        AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", newTarget);

       
    }
}

void ASAiCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributesComponent* OwningComp, float newHealth, float delta)
{
    if (delta < 0.0f)
    {
        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (ActiveHealthBar == nullptr)
        {
            ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

            if (ActiveHealthBar)
            {
                ActiveHealthBar->AttachedActor = this;
                ActiveHealthBar->AddToViewport();
            }
        }

        GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

        AAIController* AAIC = Cast<AAIController>(GetController());
        
        //ded
        if (newHealth <= 0.0f)
        {
            if (AAIC)
            {
                AAIC->GetBrainComponent()->StopLogic("Killed");
            }

            GetMesh()->SetAllBodiesSimulatePhysics(true);
            GetMesh()->SetCollisionProfileName("Ragdoll");

            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            SetLifeSpan(10.0f);
        }
    }
}

