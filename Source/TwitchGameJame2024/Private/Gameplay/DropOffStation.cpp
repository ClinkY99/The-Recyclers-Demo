// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/DropOffStation.h"
#include "Worker.h"

// Sets default values
ADropOffStation::ADropOffStation()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	mesh->SetCustomDepthStencilValue(0);

	overlap = CreateDefaultSubobject<UBoxComponent>(FName("Overlap"));
	overlap->SetupAttachment(mesh);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropOffStation::BeginPlay()
{
	Super::BeginPlay();
	
	overlap->OnComponentBeginOverlap.AddDynamic(this, &ADropOffStation::workerOverlap);

	overlap->OnComponentEndOverlap.AddDynamic(this, &ADropOffStation::workerLeavesOverlap);
}

// Called every frame
void ADropOffStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropOffStation::hover(bool isHovered)
{
	if (!isSelected) {
		mesh->SetRenderCustomDepth(isHovered);
	}
}

void ADropOffStation::select(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(true);
	isSelected = true;
}

void ADropOffStation::unselect(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(false);
	isSelected = false;
}

void ADropOffStation::startRound()
{
	inRound = true;
}

void ADropOffStation::endRound()
{
	inRound = false;
}

void ADropOffStation::workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWorker* workerOverlap = Cast<AWorker>(OtherActor)) {
		
		workers.Add(workerOverlap);

		workerOverlap->doneWorking = false;

		workerOnStation = true;

		GetWorld()->GetTimerManager().SetTimer(workTimer, this, &ADropOffStation::work, (defaultUnloadTime + defaultUnloadTime * (1 - workerOverlap->workEfficiency)) * (EfficiencyBonus), false);
	}
}

void ADropOffStation::workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AWorker* worker = Cast<AWorker>(OtherActor)) {
		workers.Remove(worker);
		if (workers.Num() == 0) {
			workerOnStation = false;
		}
	}
}

void ADropOffStation::work()
{
	if (workers.Num() > 0) {
		if (owner->dropOffMaterial(workers[0]->containerOfMaterials)) {
			workers[0]->doneWorking = true;
			workers[0]->containerOfMaterials = nullptr;
		}

		workers[0]->doneWorking = true;
	}

	//owner do stuff
}

