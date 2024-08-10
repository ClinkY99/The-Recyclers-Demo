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
	if (containers.Num()!=0) {
		materialType = containers[0]->materialType;
		containers.RemoveAt(0, EAllowShrinking::Yes);
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

