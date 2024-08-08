// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/CubeStorage.h"
#include "Engine/StaticMeshActor.h"
#include "../MainCharacter.h"
#include "Kismet/GameplayStatics.h"

ACubeStorage::ACubeStorage()
{
	FirstSpawnSpot = CreateDefaultSubobject<USceneComponent>(FName("firstCubeSpawnLocation"));
	FirstSpawnSpot->SetupAttachment(mesh);

	dropOffPoint = CreateDefaultSubobject<USceneComponent>(FName("DropOffLocation"));
	dropOffPoint->SetupAttachment(mesh);

	workStationPoint = CreateDefaultSubobject<USceneComponent>(FName("WorkStationLocation"));
	workStationPoint->SetupAttachment(mesh);
}

void ACubeStorage::BeginPlay()
{
	Super::BeginPlay();
	if (!inputBuilding) {
		spawnDropOff();
	}
	spawnWorkstation();

}


bool ACubeStorage::dropOffMaterial(Container* container)
{
	if (Super::dropOffMaterial(container)) {
		AStaticMeshActor* newMesh = GetWorld()->SpawnActor<AStaticMeshActor>();
		newMesh->GetStaticMeshComponent()->SetStaticMesh(GarbageCubeMesh);
		newMesh->GetStaticMeshComponent()->SetMaterial(0, Materials[container->materialType - 4]);
		newMesh->AttachToComponent(FirstSpawnSpot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FVector newLocation = newMesh->GetActorLocation();
		if (GetActorRotation().Yaw == 0, 180) {
			newLocation.Y += spawned > 2 ? 250 * spawned - 3 : 250 * spawned;
		}
		else {
			newLocation.X += spawned > 2 ? 250 * spawned - 3 : 250 * spawned;
		}
		newLocation.Z = spawned > 2 ? 300 : 100;

		SpawnTruck();

		return true;
	}
	return false;
}

void ACubeStorage::SpawnTruck()
{
	int32 num = FMath::RandRange(0, 100);
	if (spawned == 4) {
		if (num < 10) {
			Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
		}
	}
	if (spawned == 5) {
		if (num < 33) {
			Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
		}
	} 
	if (spawned > 5) {
		Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SpawnCollectionTruck();
	}
}


void ACubeStorage::spawnDropOff()
{
	ADropOffStation* dropOffStation = GetWorld()->SpawnActor<ADropOffStation>(dropOffClass);
	dropOffStation->owner = this;
	dropOffStation->isCompactCubes = true;

	dropOffStation->AttachToComponent(dropOffPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = dropOffStation->GetActorLocation();
	location.Z = 0;
	dropOffStation->SetActorLocation(location);
}


void ACubeStorage::spawnWorkstation()
{
	AWorkstation* workstation = GetWorld()->SpawnActor<AWorkstation>(workstationClass);
	workstation->owner = this;

	workstation->AttachToComponent(workStationPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = workstation->GetActorLocation();
	location.Z = 0;
	workstation->SetActorLocation(location);
}
