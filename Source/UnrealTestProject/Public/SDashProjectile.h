// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTESTPROJECT_API ASDashProjectile : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASDashProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Teleport");
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport");
	float DetonateDelay;

	FTimerHandle TimerHandle_DelayedDetonate;
	virtual void Explode_Implementation() override;

	void TeleportInstigator();

	virtual void BeginPlay() override;
};
