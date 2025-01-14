// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NavModifierComponent.h"
#include "../StartInterface.h"
#include "BuildablesBase.generated.h"

UCLASS()
class ABuildablesBase : public AActor, public IStartInterface
{
	GENERATED_BODY()
	
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UNavModifierComponent* navBlock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* overlap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UMaterial* regularMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* HoloMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* InvalidMat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* placingMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;
public:
	UPROPERTY(BlueprintReadWrite)
	bool isInvalid;

private:
	bool inRound;

protected:
	UPROPERTY(BlueprintReadWrite)
	bool isPlaced;

public:	
	// Sets default values for this actor's properties
	ABuildablesBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void startRound() override;

	virtual void endRound() override;

public:
	UFUNCTION()
	void makeHolo();

	UFUNCTION()
	void removeHolo();

	UFUNCTION()
	void makeInvalid();

	virtual void build();

	virtual void destroyBuilding();

protected:


private:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	FORCEINLINE UStaticMeshComponent* getMesh() { return mesh; };
};

