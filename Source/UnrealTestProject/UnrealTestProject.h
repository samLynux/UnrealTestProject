// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("STANFORD_Game"), STATGROUP_STANFORD, STATCAT_Advanced);

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	UWorld* world = WorldContext->GetWorld();
	if (!ensure(world))
	{
		return;
	}

	FString NetPrefix = world->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, duration, Color, NetPrefix + Msg);
	}
}