// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../StartInterface.h"
#include "../InteractInterface.h"
#include "../Buildings/AutomatedBuilding.h"
#include "DropOffStation.generated.h"

UCLASS()
class ADropOffStation : public AActor, public IStartInterface, public IInteractInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* overlap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workTime, meta = (AllowPrivateAccess = "true"))
	float defaultUnloadTime;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = behaviorTree)
	float EfficiencyBonus = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Working)
	bool workerOnStation;

	AAutomatedBuilding* owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Working)
	bool isCompactCubes;

protected:
	bool inRound;
	

	TArray<class AWorker*> workers;

private:
	FTimerHandle workTimer;

	bool isSelected;


	
public:	
	// Sets default values for this actor's properties
	ADropOffStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void hover(bool isHovered) override;

	virtual void select(class AMainCharacter* character) override;

	virtual void unselect(class AMainCharacter* character) override;

	virtual void startRound() override;

	virtual void endRound() override;

private:
	UFUNCTION()
	void workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void work();

};
