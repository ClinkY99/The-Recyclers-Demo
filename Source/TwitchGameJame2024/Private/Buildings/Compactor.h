// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/AutomatedBuilding.h"
#include "../Gameplay/DropOffStation.h"
#include "../Gameplay/compactingStation.h"
#include "../Items/CompressedCube.h"
#include "Compactor.generated.h"

/**
 * 
 */
UCLASS()
class ACompactor : public AAutomatedBuilding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	USceneComponent* dropOffPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropOffStation> dropOffClass;

	ADropOffStation* dropOffStation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = compressing, meta = (AllowPrivateAccess = "true"))
	int32 numContainersRequired;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = compressing, meta = (AllowPrivateAccess = "true"))
	float compressionTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = compressing, meta = (AllowPrivateAccess = "true"))
	int32 maxAmountInOutput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = compressing, meta = (AllowPrivateAccess = "true"))
	bool isManual;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	USceneComponent* workStationPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AcompactingStation> workstationClass;

	AcompactingStation* workstation;



private:
	bool isCompressing;
	int32 materialType;

	FTimerHandle compressionTimer;

public:
	TArray<CompressedCube*> ItemsInOutput;

public:
	ACompactor();

protected:

	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void build() override;

	void abortCompacting();

	virtual void destroyBuilding() override;

private:
	void compress();
	void finishCompressing();

	void spawnDropOff();

	void spawnWorkstation();
	
	
};
