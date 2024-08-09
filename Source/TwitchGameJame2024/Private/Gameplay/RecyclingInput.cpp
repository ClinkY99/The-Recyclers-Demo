// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RecyclingInput.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../GarbadgeTruck.h"

// Sets default values
ARecyclingInput::ARecyclingInput()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetupAttachment(RootComponent);

	mesh->SetCustomDepthStencilValue(0);

	navBlock = CreateDefaultSubobject<UNavModifierComponent>(FName("Nav Modifier"));

	plasticWorkstation = CreateDefaultSubobject<USceneComponent>(FName("Plastic Workstations Spawn Loc"));
	plasticWorkstation->SetupAttachment(mesh);

	coloredPlasticWorkstation = CreateDefaultSubobject<USceneComponent>(FName("colored Plastic Workstation Spawn Loc"));
	coloredPlasticWorkstation->SetupAttachment(mesh);

	cardboardWorkstation = CreateDefaultSubobject<USceneComponent>(FName("Cardboard Workstations Spawn Loc"));
	cardboardWorkstation->SetupAttachment(mesh);

	cansWorkstation = CreateDefaultSubobject<USceneComponent>(FName("Cans Workstations Spawn Loc"));
	cansWorkstation->SetupAttachment(mesh);

	dropOffZone = CreateDefaultSubobject<UBoxComponent>(FName("Overlap"));
	dropOffZone->SetupAttachment(mesh);

	teir = 1;

	spacing = 100;

	maxRecyclingBeforeOverflow = 20;
	TurnsExisted = 1;

	
}

// Called when the game starts or when spawned
void ARecyclingInput::BeginPlay()
{
	Super::BeginPlay();

	spawnWorkstations(plasticWorkstation);
	resources.Init(0, 4);
	resourcesOverflow.Init(false, 4);

	dropOffZone->OnComponentEndOverlap.AddDynamic(this, &ARecyclingInput::truckDropsOff);
	
}

// Called every frame
void ARecyclingInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (size_t i = 0; i < teir; i++)
	{
		if (resources[i] >= maxRecyclingBeforeOverflow && !overflowTimer.IsValid()) {
			resourcesOverflow[i] = true;
			GetWorld()->GetTimerManager().SetTimer(overflowTimer, this, &ARecyclingInput::overflow, overFlowTime, false);

			mesh->SetRenderCustomDepth(true);
			mesh->SetCustomDepthStencilValue(1);

		}
		else if (resourcesOverflow[i]) {
			resourcesOverflow[i] = false;
			GetWorld()->GetTimerManager().ClearTimer(overflowTimer);
			mesh->SetRenderCustomDepth(false);
			mesh->SetCustomDepthStencilValue(0);

		}
	}
}

void ARecyclingInput::hover(bool isHovered)
{
	if (!isSelected&& !GetWorldTimerManager().IsTimerActive(overflowTimer)) {
		mesh->SetRenderCustomDepth(isHovered);
	}
}

void ARecyclingInput::select(AMainCharacter* character)
{
	mesh->SetRenderCustomDepth(true);
	isSelected = true;
	showInfoWidget();
}

void ARecyclingInput::unselect(AMainCharacter* character)
{
	if (!GetWorldTimerManager().IsTimerActive(overflowTimer)) {
		mesh->SetRenderCustomDepth(false);
	}
	isSelected = false;
	closeInfoWidget();
}

void ARecyclingInput::truckArrives()
{
	for (size_t i = 0; i < teir; i++)
	{
		resources[i] += FMath::RandRange(1, 4);
		
	}
}

void ARecyclingInput::overflow()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (AMainCharacter* characterRef = Cast<AMainCharacter>(character)) {
		characterRef->endGame();
	}
}

void ARecyclingInput::truckDropsOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AGarbadgeTruck>(OtherActor)) {
		truckArrives();
	}
}

void ARecyclingInput::startRound()
{

}

void ARecyclingInput::endRound()
{
	GetWorldTimerManager().ClearTimer(timer);
	GetWorldTimerManager().ClearTimer(overflowTimer);
}

void ARecyclingInput::upgrade()
{

}

void ARecyclingInput::addResource()
{
	if (teir == 1) {
		spawnWorkstations(coloredPlasticWorkstation);
	}
	else if (teir == 2) {
		spawnWorkstations(cardboardWorkstation);
	}
	else if (teir == 3) {
		spawnWorkstations(cansWorkstation);
	}
	teir++;
}

void ARecyclingInput::spawnWorkstations(USceneComponent* AttachPoint)
{
	ASortingStation* work1 = GetWorld()->SpawnActor<ASortingStation>(workstationClass);
	work1->owner = this;
	work1->material = teir - 1;

	ASortingStation* work2 = GetWorld()->SpawnActor<ASortingStation>(workstationClass);
	work2->owner = this;
	work2->material = teir - 1;

	work1->AttachToComponent(AttachPoint, FAttachmentTransformRules::KeepWorldTransform);
	work2->AttachToComponent(AttachPoint, FAttachmentTransformRules::KeepWorldTransform);

	FVector vector1 = AttachPoint->GetRelativeLocation() + GetActorLocation();
	vector1.X -= spacing;
	work1->SetActorLocation(vector1);


	FVector vector2 = AttachPoint->GetRelativeLocation() + GetActorLocation();
	vector2.X += spacing;
	work2->SetActorLocation(vector2);

	workstations.Add(work1);
	workstations.Add(work2);
}


