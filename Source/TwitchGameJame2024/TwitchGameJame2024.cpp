// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwitchGameJame2024.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, TwitchGameJame2024, "TwitchGameJame2024" );

FVector snapToGrid2d(FVector vector, float grid, int32 zHeight)
{
	FVector final;

	final.X = int(vector.X / grid) * grid;
	final.Y = int(vector.Y / grid) * grid;
	final.Z = zHeight;

	return final;
}
