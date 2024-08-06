// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractInterface.h"
#include "Workstation.generated.h"

class AWorker;

UCLASS()
class AWorkstation : public AActor, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;
	
private:
	bool isSelected;
	AWorker* worker;

public:	
	// Sets default values for this actor's properties
	AWorkstation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void hover(bool isHovered) override;

	virtual void select(AMainCharacter* character) override;

	virtual void unselect(AMainCharacter* character) override;

public:
	FORCEINLINE AWorker* GetWorker() { return worker; };

	FORCEINLINE void SetWorker(AWorker* newWorker) { worker = newWorker; };

};
