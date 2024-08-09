// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/Compactor.h"

ACompactor::ACompactor()
{
	dropOffPoint = CreateDefaultSubobject<USceneComponent>(FName("DropOffLocation"));
	dropOffPoint->SetupAttachment(mesh);

	workStationPoint = CreateDefaultSubobject<USceneComponent>(FName("WorkStationLocation"));
	workStationPoint->SetupAttachment(mesh);

	
}

void ACompactor::BeginPlay()
{
	Super::BeginPlay();	
}

void ACompactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isCompressing) {
		compress();
	}
	if (ItemsInOutput.Num()>0 && outputBuilding) {
		if (outputBuilding->dropOffcompressedCube(ItemsInOutput[0])) {
			ItemsInOutput.RemoveAt(0);
		}
	}
}

void ACompactor::build()
{
	if (!inputBuilding) {
		spawnDropOff();
	}
	if (isManual) {
		spawnWorkstation();
	}
}

void ACompactor::abortCompacting()
{
	if (isCompressing) {
		GetWorldTimerManager().ClearTimer(compressionTimer);
	}
}

void ACompactor::destroyBuilding()
{
	workstation->destroyWorkstation();
	if (dropOffStation->Destroy()) {
		Super::destroyBuilding();
	}
	
}

void ACompactor::compress()
{
	TArray<Container*> containerSearch = TArray<Container*>(containers);
	TArray<Container*> containersFound;

	for (auto& firstContainer : containerSearch)
	{
		containersFound.Add(firstContainer);
		if (containersFound.Num() != numContainersRequired) {
			for (auto& container : containerSearch) {
				if (container->materialType == containersFound[0]->materialType) {
					containersFound.Add(container);
					if (containersFound.Num() == numContainersRequired) {
						break;
					}
				}
			}
			if (containersFound.Num() != numContainersRequired) {
				for (auto& container : containersFound) {
					containerSearch.Remove(container);
				}
				containersFound.Empty();
			}
			else {
				break;
			}
		}
	}
	if (containersFound.Num() > 0) {
		materialType = containersFound[0]->materialType;
		for (auto& containersUsed : containersFound) {
			containers.Remove(containersUsed);
		}
		isCompressing = true;
		GetWorldTimerManager().SetTimer(compressionTimer, this, &ACompactor::finishCompressing, compressionTime, false);
	}	
}

void ACompactor::finishCompressing()
{
	isCompressing = false;

	CompressedCube* compressedCube = new CompressedCube();
	compressedCube->materialType = materialType;

	ItemsInOutput.Add(compressedCube);
}

void ACompactor::spawnDropOff()
{
	dropOffStation = GetWorld()->SpawnActor<ADropOffStation>(dropOffClass);
	dropOffStation->owner = this;

	dropOffStation->AttachToComponent(dropOffPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = dropOffStation->GetActorLocation();
	location.Z = 0;
	dropOffStation->SetActorLocation(location);
}


void ACompactor::spawnWorkstation()
{
	workstation = GetWorld()->SpawnActor<AcompactingStation>(workstationClass);
	workstation->owner = this;

	workstation->AttachToComponent(workStationPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = workstation->GetActorLocation();
	location.Z = 0;
	workstation->SetActorLocation(location);
}

