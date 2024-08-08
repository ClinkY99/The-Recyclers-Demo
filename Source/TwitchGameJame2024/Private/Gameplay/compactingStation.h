// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Workstation.h"

#include "compactingStation.generated.h"

/**
 * 
 */
UCLASS()
class AcompactingStation : public AWorkstation
{
	GENERATED_BODY()
	
public:
	class ACompactor* owner;

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
