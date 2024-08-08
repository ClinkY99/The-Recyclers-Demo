// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildings/BuildablesBase.h"
#include"../Container.h"
#include "../Items/CompressedCube.h"
#include "AutomatedBuilding.generated.h"

/**
 * 
 */
UCLASS()
class AAutomatedBuilding : public ABuildablesBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = automation, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* inputBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = automation, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* outputBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = automation, meta = (AllowPrivateAccess = "true"))
	int32 maxNumContainers;

	TArray<Container*> containers;

	bool knowsType;

	AAutomatedBuilding* inputBuilding;

	AAutomatedBuilding* outputBuilding;

public:
	AAutomatedBuilding();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void validateOutput(AAutomatedBuilding* buildingRef);

	UFUNCTION()
	virtual void validateInput(AAutomatedBuilding* buildingRef);

public:
	virtual bool dropOffMaterial(Container* container);
	virtual bool dropOffcompressedCube(CompressedCube* compressedCube);
	
};
