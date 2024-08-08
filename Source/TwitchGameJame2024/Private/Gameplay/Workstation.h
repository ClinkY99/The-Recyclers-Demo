// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractInterface.h"
#include "../StartInterface.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Workstation.generated.h"

class AWorker;

UCLASS()
class AWorkstation : public AActor, public IInteractInterface, public IStartInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* overlap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = behaviorTree, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* behaviorTreeToRun;
	
protected:
	bool isSelected;
	bool inRound;
	bool workerOnStation;
	AWorker* worker;

public:
	AActor* owner;



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

	UFUNCTION(BlueprintCallable)
	virtual void select(AMainCharacter* character) override;
	UFUNCTION(BlueprintCallable)
	virtual void unselect(AMainCharacter* character) override;

	virtual void startRound() override;

	virtual void endRound() override;

	UFUNCTION(BlueprintCallable)
	virtual void workerRelocated();

protected:
	UFUNCTION()
	virtual void workerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void workerLeavesOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE AWorker* GetWorker() { return worker; };

	FORCEINLINE void SetWorker(AWorker* newWorker) { worker = newWorker; };

};
