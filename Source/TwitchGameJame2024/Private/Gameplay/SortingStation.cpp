// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SortingStation.h"
#include "../Worker.h"
#include "RecyclingInput.h"

void ASortingStation::work()
{
	if (inRound) {
		if (owner->resources[container->materialType] != 0 && container->amountOfMaterial < defaultCarryingCapacity) {
			owner->resources[container->materialType]--;
			container->amountOfMaterial++;
		}
		if (container->amountOfMaterial == defaultCarryingCapacity * teir) {
			worker->doneWorking = true;
			worker->containerOfMaterials = container;
		}
	}
}

void ASortingStation::workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWorker* workerOverlap = Cast<AWorker>(OtherActor)) {
		Super::workerOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		if (workerOverlap == worker) {
			worker->doneWorking = false;
			container = new Container();
			container->materialType = material;
			GetWorldTimerManager().SetTimer(workTimer, this, &ASortingStation::work, (defaultWorkTime + defaultWorkTime * (1 - worker->workEfficiency)) * (EfficiencyBonus / teir), true);
		}
	}
}

void ASortingStation::workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::workerLeavesOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (Cast<AWorker>(OtherActor) == worker) {
		GetWorldTimerManager().ClearTimer(workTimer);
	}

}

void ASortingStation::workerRelocated()
{
	Super::workerRelocated();
	GetWorldTimerManager().ClearTimer(workTimer);
}
