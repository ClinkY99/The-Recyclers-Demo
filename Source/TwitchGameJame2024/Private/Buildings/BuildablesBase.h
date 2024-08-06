// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NavModifierComponent.h"
#include "BuildablesBase.generated.h"

UCLASS()
class ABuildablesBase : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

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

public:
	UPROPERTY(BlueprintReadWrite)
	bool isInvalid;

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

public:
	UFUNCTION()
	void makeHolo();

	UFUNCTION()
	void removeHolo();

	UFUNCTION()
	void makeInvalid();

private:
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
