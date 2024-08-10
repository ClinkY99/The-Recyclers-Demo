// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractInterface.h"
#include "../StartInterface.h"
#include "SortingStation.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "RecyclingInput.generated.h"

UCLASS()
class ARecyclingInput : public AActor, public IInteractInterface, public IStartInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UNavModifierComponent* navBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	USceneComponent* plasticWorkstation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	USceneComponent* coloredPlasticWorkstation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	USceneComponent* cardboardWorkstation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	USceneComponent* cansWorkstation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASortingStation> workstationClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	int32 spacing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	float resourceAccumilationTimeBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	float overFlowTime;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* UpgradedMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* dropOffZone;

	

private:
	bool isSelected;

	TArray<AWorkstation*> workstations;

	FTimerHandle timer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = mesh)
	TSubclassOf<UUserWidget> widgetUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = diffuculty)
	float TurnsExisted;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = overflow)
	FTimerHandle overflowTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> resourcesOverflow;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<int32> resources;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 teir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 minGardbage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 maxGardbage = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = workstations, meta = (AllowPrivateAccess = "true"))
	int32 maxRecyclingBeforeOverflow;


public:	
	// Sets default values for this actor's properties
	ARecyclingInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void hover(bool isHovered) override;

	virtual void select(AMainCharacter* character) override;

	virtual void unselect(AMainCharacter* character) override;

	virtual void startRound() override;

	virtual void endRound() override;

private:

	UFUNCTION()
	void overflow();

	UFUNCTION()
	virtual void truckDropsOff(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void showInfoWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void closeInfoWidget();

public:

	UFUNCTION()
	void truckArrives();

	UFUNCTION(BlueprintCallable)
	void upgrade();

	UFUNCTION(BlueprintCallable)
	void addResource();
	
	UFUNCTION(BlueprintCallable)
	void spawnWorkstations(USceneComponent* AttachPoint);

};
