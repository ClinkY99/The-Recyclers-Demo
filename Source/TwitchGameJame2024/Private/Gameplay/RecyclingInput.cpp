// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RecyclingInput.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	
}

// Called every frame
void ARecyclingInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARecyclingInput::hover(bool isHovered)
{
	if (!isSelected) {
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
	mesh->SetRenderCustomDepth(false);
	isSelected = false;
	closeInfoWidget();
}

void ARecyclingInput::truckArrives()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Generate Plastic"));
	for (size_t i = 0; i < teir; i++)
	{
		resources[i] += FMath::RandRange(1, 4);
		if (resources[i] > maxRecyclingBeforeOverflow && !overflowTimer.IsValid()) {
			GetWorld()->GetTimerManager().SetTimer(overflowTimer, this, &ARecyclingInput::overflow, overFlowTime, false);
			
		}
		if (resources[i] > maxRecyclingBeforeOverflow) {
			resources[i] = 20;
		}
	}
}

void ARecyclingInput::overflow()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (AMainCharacter* characterRef = Cast<AMainCharacter>(character)) {
		characterRef->endGame();
	}
}

void ARecyclingInput::startRound()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Round Started"));
	
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ARecyclingInput::truckArrives, FMath::Clamp(resourceAccumilationTimeBase/TurnsExisted, 3,60), true);
}

void ARecyclingInput::endRound()
{
	GetWorldTimerManager().ClearTimer(timer);
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


