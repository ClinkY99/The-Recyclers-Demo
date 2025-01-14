// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Private/Buildings/BuildablesBase.h"
#include "TwitchGameJame2024.generated.h"

class AContracts_base;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 cost;
};

USTRUCT(BlueprintType)
struct FContractStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AContracts_base> classRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 moneyBonus;
};

UENUM(BlueprintType)
enum ERecyclingType {
	Plastic UMETA(DisplayName = "Plastic"),
	ColoredPlastic UMETA(DisplayName = "ColoredPlastic"),
	Cardboard UMETA(DisplayName = "Cardboard"),
	Cans UMETA(DisplayName = "Cans")
};

UENUM(BlueprintType)
enum EMaterialType{
	CompressedCubePlasticMetal UMETA(DisplayName = "CompressedCubePlasticMetal"),
	CompressedCubePaperCardboard UMETA(DisplayName = "CompressedCubePlasticMetal")
};

FVector snapToGrid2d(FVector vector, float grid, int32 zHeight);
