// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpActor.h"
#include "SPowerUpCredits.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTESTPROJECT_API ASPowerUpCredits : public ASPowerUpActor
{
	GENERATED_BODY()

public:
	ASPowerUpCredits();

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CreditAmount;

	void Interact_Implementation(APawn* InstigatorPawn) override;

};
