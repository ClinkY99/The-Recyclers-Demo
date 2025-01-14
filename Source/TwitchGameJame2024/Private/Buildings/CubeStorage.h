// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/AutomatedBuilding.h"
#include "../Gameplay/DropOffStation.h"
#include "../Gameplay/Workstation.h"
#include "CubeStorage.generated.h"

/**
 * 
 */
UCLASS()
class ACubeStorage : public AAutomatedBuilding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	USceneComponent* dropOffPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropOffStation> dropOffClass;
	
	ADropOffStation* dropOffStation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterial*> Materials;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	UMaterial* secondMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* GarbageCubeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirstSpawnSpot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	int32 maxSizeAmount;

public:
	TArray<UStaticMeshComponent*> cubes;

private:
	int32 spawned;

	TArray<UStaticMeshComponent*> compressedCubes;

public:
	ACubeStorage();
protected:
	virtual void BeginPlay() override;

public:

	virtual void build() override;

	virtual void destroyBuilding() override;

	virtual bool dropOffMaterial(Container* container) override;

	void removeCubes();

private:

	void spawnDropOff();


};
