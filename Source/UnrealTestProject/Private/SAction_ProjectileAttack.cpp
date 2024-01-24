// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include <DrawDebugHelpers.h>

USAction_ProjectileAttack::USAction_ProjectileAttack()
{

	AttackAnimDelay = 0.2f;
	
	HandSocketName = "Muzzle_01";
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	ACharacter* character = Cast<ACharacter>(Instigator);
	if (character)
	{
		character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		
		if (character->HasAuthority())
		{
			FTimerHandle timerHandle_AttackDelay;
			FTimerDelegate delegate;
			delegate.BindUFunction(this, "AttackDelay_Elapsed", character);

			GetWorld()->GetTimerManager().SetTimer(timerHandle_AttackDelay, delegate, AttackAnimDelay, false);
		}



	}


	
}


void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* Instigator)
{
	
	if (ensureAlways(ProjectileClass))
	{
		FVector handLoc = Instigator->GetMesh()->GetSocketLocation("Muzzle_01");



		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = Instigator;

		FCollisionShape shape;
		shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);

		FCollisionObjectQueryParams ObjParam;
		ObjParam.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParam.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParam.AddObjectTypesToQuery(ECC_Pawn);

		FVector traceStart = Instigator->GetPawnViewLocation();
		FVector traceEnd = traceStart + (Instigator->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		if (GetWorld()->SweepSingleByObjectType(Hit, traceStart, traceEnd, FQuat::Identity, ObjParam, shape, Params))
		{
			traceEnd = Hit.ImpactPoint;
		}

		FRotator projRotation = FRotationMatrix::MakeFromX(traceEnd - handLoc).Rotator();

		FTransform spawnTM = FTransform(projRotation, handLoc);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, spawnTM, SpawnParams);
	}

	StopAction(Instigator);
}


