// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/BuildablesBase.h"

// Sets default values
ABuildablesBase::ABuildablesBase()
{

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	placingMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("placingMesh"));
	placingMesh->SetupAttachment(mesh);

	overlap = CreateDefaultSubobject<UBoxComponent>(FName("Overlap"));
	overlap->SetupAttachment(mesh);

	navBlock = CreateDefaultSubobject<UNavModifierComponent>(FName("Nav Modifier"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isInvalid = false;
	isPlaced = false;
}

// Called when the game starts or when spawned
void ABuildablesBase::BeginPlay()
{
	Super::BeginPlay();

	placingMesh->SetVisibility(false);

	overlap->OnComponentBeginOverlap.AddDynamic(this, &ABuildablesBase::OverlapBegin);

	overlap->OnComponentEndOverlap.AddDynamic(this, &ABuildablesBase::OverlapEnd);	
}

// Called every frame
void ABuildablesBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	overlap->SetWorldLocation(GetActorLocation());

}

void ABuildablesBase::startRound()
{
	inRound = true;
}

void ABuildablesBase::endRound()
{
	inRound = false;
}

void ABuildablesBase::makeHolo()
{
	mesh->SetVisibility(false);
	placingMesh->SetVisibility(true);
	placingMesh->SetMaterial(0,HoloMat);
	isInvalid = false;
}

void ABuildablesBase::removeHolo()
{
	mesh->SetVisibility(true);
	placingMesh->SetVisibility(false);
	isPlaced = true;
	build();
}

void ABuildablesBase::makeInvalid()
{
	placingMesh->SetVisibility(true);
	placingMesh->SetMaterial(0, InvalidMat);
	isInvalid = true;
}

void ABuildablesBase::build()
{

}

void ABuildablesBase::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABuildablesBase>(OtherActor) && !isPlaced) {
		makeInvalid();
	}
}

void ABuildablesBase::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ABuildablesBase>(OtherActor)&&!isPlaced) {
		makeHolo();
	}
}

