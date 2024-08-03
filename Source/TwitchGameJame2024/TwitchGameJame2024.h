// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Private/Buildings/BuildablesBase.h"
#include "TwitchGameJame2024.generated.h"

USTRUCT(BlueprintType)
struct FBuildableStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABuildablesBase> classRef;
};

FVector snapToGrid2d(FVector vector, float grid, int32 zHeight);
