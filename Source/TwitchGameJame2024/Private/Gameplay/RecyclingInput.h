// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../InteractInterface.h"
#include "RecyclingInput.generated.h"

UCLASS()
class ARecyclingInput : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* mesh;

private:
	bool isSelected;

	TArray<int32> resources;

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

public:
	UFUNCTION(BlueprintCallable)
	void startRound();

	UFUNCTION(BlueprintCallable)
	void endRound();

	UFUNCTION(BlueprintCallable)
	void upgrade();

	UFUNCTION(BlueprintCallable)
	void addResource();
};
