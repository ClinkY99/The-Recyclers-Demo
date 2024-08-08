// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/compactingStation.h"
#include "../Worker.h"
#include "../Buildings/Compactor.h"

void AcompactingStation::work()
{
	if (owner->ItemsInOutput.Num() > 0) {
		Container* container = new Container();
		container->materialType = owner->ItemsInOutput[0]->materialType + 4;
		container->amountOfMaterial = 1;
		worker->containerOfMaterials = container;
		worker->doneWorking = true;
		owner->ItemsInOutput.Remove(owner->ItemsInOutput[0]);
	}
}

void AcompactingStation::workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWorker* workerOverlap = Cast<AWorker>(OtherActor)) {
		Super::workerOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		if (workerOverlap == worker) {
			worker->doneWorking = false;
			GetWorldTimerManager().SetTimer(workTimer, this, &AcompactingStation::work, 1, true);
		}
	}
}

void AcompactingStation::workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::workerLeavesOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (Cast<AWorker>(OtherActor) == worker) {
		GetWorldTimerManager().ClearTimer(workTimer);
	}
}

void AcompactingStation::workerRelocated()
{
	Super::workerRelocated();
	GetWorldTimerManager().ClearTimer(workTimer);
}
