// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerUpActor.h"
#include "SPowerUp_HealthPotion.generated.h"


class UMeshComponent;

UCLASS()
class UNREALTESTPROJECT_API ASPowerUp_HealthPotion : public ASPowerUpActor
{
	GENERATED_BODY()
	
protected:

public:
	ASPowerUp_HealthPotion();

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CreditCost;

	void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};
