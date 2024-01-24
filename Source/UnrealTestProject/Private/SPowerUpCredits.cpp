// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUpCredits.h"
#include "SPlayerState.h"

ASPowerUpCredits::ASPowerUpCredits()
{
	CreditAmount = 80;
}

void ASPowerUpCredits::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		PS->AddCredits(CreditAmount);
		HideAndCooldownPowerup();
	}
}
