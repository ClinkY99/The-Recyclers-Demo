// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildablesBase.h"

// Sets default values
ABuildablesBase::ABuildablesBase()
{

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	overlap = CreateDefaultSubobject<UBoxComponent>(FName("Overlap"));
	overlap->SetupAttachment(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isOverlap = false;
	isPlaced = false;
}

// Called when the game starts or when spawned
void ABuildablesBase::BeginPlay()
{
	Super::BeginPlay();

	overlap->OnComponentBeginOverlap.AddDynamic(this, &ABuildablesBase::OverlapBegin);

	overlap->OnComponentEndOverlap.AddDynamic(this, &ABuildablesBase::OverlapEnd);
	
}

// Called every frame
void ABuildablesBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	overlap->SetWorldLocation(GetActorLocation());

}

void ABuildablesBase::makeHolo()
{
	mesh->SetMaterial(0, HoloMat);
	isOverlap = false;
}

void ABuildablesBase::removeHolo()
{
	mesh->SetMaterial(0, regularMat);
	isPlaced = true;
}

void ABuildablesBase::makeOverlap()
{
	mesh->SetMaterial(0, OverlapMat);
	isOverlap = true;
}

void ABuildablesBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABuildablesBase>(OtherActor) && !isPlaced) {
		makeOverlap();
	}
}

void ABuildablesBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ABuildablesBase>(OtherActor)&&!isPlaced) {
		makeHolo();
	}
}

