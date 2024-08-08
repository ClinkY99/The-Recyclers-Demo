// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Workstation.h"
#include "../Container.h"
#include "SortingStation.generated.h"

/**
 * 
 */
UCLASS()
class ASortingStation : public AWorkstation
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	float defaultWorkTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	int32 defaultCarryingCapacity;

public:
	class ARecyclingInput* owner;

	int32 teir = 1;

	int32 EfficiencyBonus = 1;

	int32 material;

	Container* container;

private:
	FTimerHandle workTimer;

private:
	void work();

protected:
	virtual void workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
public:
	virtual void workerRelocated() override;
};
