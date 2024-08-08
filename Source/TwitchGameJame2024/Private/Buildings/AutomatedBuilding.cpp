// Fill out your copyright notice in the Description page of Project Settings.


#include "Buildings/AutomatedBuilding.h"

AAutomatedBuilding::AAutomatedBuilding()
{
	inputBox = CreateDefaultSubobject<UBoxComponent>(FName("Input Box"));
	inputBox->SetupAttachment(mesh);

	outputBox = CreateDefaultSubobject<UBoxComponent>(FName("Output Box"));
	outputBox->SetupAttachment(mesh);
}

void AAutomatedBuilding::BeginPlay()
{
	Super::BeginPlay();

	TSet<UPrimitiveComponent*> overlappingComponents;

	inputBox->GetOverlappingComponents(overlappingComponents);

	for (auto& component : overlappingComponents) {
		if (component->GetName() == "Output Box") {
			if (AAutomatedBuilding* actor = Cast<AAutomatedBuilding>(component->GetOwner())) {
				actor->validateOutput(this);
				validateInput(actor);
			}
		}
		else if (component->GetName() == "Input Box") {
			if (AAutomatedBuilding* actor = Cast<AAutomatedBuilding>(component->GetOwner())) {
				validateOutput(actor);
				actor->validateInput(this);
			}
		}
	}
	
}

bool AAutomatedBuilding::dropOffMaterial(Container* container)
{
	if (containers.Num() < maxNumContainers) {
		containers.Add(container);
		return true;
	}
	else {
		return false;
	}
}

bool AAutomatedBuilding::dropOffcompressedCube(CompressedCube* compressedCube)
{
	if (containers.Num() < maxNumContainers) {
		Container* container = new Container();
		container->amountOfMaterial = 1;
		container->materialType = compressedCube->materialType + 4;

		containers.Add(container);
		return true;
	}
	else {
		return false;
	}
}

void AAutomatedBuilding::validateOutput(AAutomatedBuilding* buildingRef)
{
	outputBuilding = buildingRef;
}

void AAutomatedBuilding::validateInput(AAutomatedBuilding* buildingRef)
{
	inputBuilding = buildingRef;
}
