// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../TwitchGameJame2024.h"
#include "Contracts_base.generated.h"

UCLASS()
class AContracts_base : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = dropOffs, meta = (AllowPrivateAccess = "true"))
	FContractStruct contractDetails;
	
public:	
	// Sets default values for this actor's properties
	AContracts_base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void picked() {};

	UFUNCTION(BlueprintImplementableEvent)
	void BPLogicPicked();

};
