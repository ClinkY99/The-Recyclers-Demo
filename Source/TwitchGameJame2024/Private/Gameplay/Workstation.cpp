// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/Workstation.h"
#include "Worker.h"


// Sets default values
AWorkstation::AWorkstation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	mesh->SetCustomDepthStencilValue(0);

	overlap = CreateDefaultSubobject<UBoxComponent>(FName("Overlap"));
	overlap->SetupAttachment(mesh);

}

// Called when the game starts or when spawned
void AWorkstation::BeginPlay()
{
	Super::BeginPlay();
	
	overlap->OnComponentBeginOverlap.AddDynamic(this, &AWorkstation::workerOverlap);

	overlap->OnComponentEndOverlap.AddDynamic(this, &AWorkstation::workerLeavesOverlap);
}

// Called every frame
void AWorkstation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorkstation::hover(bool isHovered)
{
	if (!isSelected) {
		mesh->SetRenderCustomDepth(isHovered);
	}
}

void AWorkstation::select(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(true);
	isSelected = true;
}

void AWorkstation::unselect(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(false);
	isSelected = false;
}

void AWorkstation::startRound()
{
	inRound = true;
}

void AWorkstation::endRound()
{
	inRound = false;
}

void AWorkstation::workerRelocated()
{
	worker = nullptr;
}

void AWorkstation::destroyWorkstation()
{
	if (worker) {
		worker->resetWorkstation();
	}
	Destroy();
}

void AWorkstation::workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AWorker* workerOverlap = Cast<AWorker>(OtherActor)) {
		if (workerOverlap == worker) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Overlap"));

			worker->doneWorking = false;

			workerOnStation = true;

			worker->updateWorkBehavior(behaviorTreeToRun);
		}
	}
}

void AWorkstation::workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AWorker>(OtherActor)) {
		workerOnStation = false;
	}
}


