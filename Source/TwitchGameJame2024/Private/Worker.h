// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractInterface.h"
#include "StartInterface.h"
#include "Gameplay/Workstation.h"
#include "NavModifierComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Container.h"
#include "Worker.generated.h"


UCLASS()
class AWorker : public ACharacter, public IInteractInterface, public IStartInterface
{
	GENERATED_BODY()
	

protected:

	UPROPERTY(BlueprintReadWrite, Category = selection)
	bool isSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = work)
	AWorkstation* workstation;
public:
	float workEfficiency = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = work)
	bool doneWorking;

	Container* containerOfMaterials;

private:
	bool inRound;

public:
	// Sets default values for this character's properties
	AWorker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void hover(bool isHovered) override;

	virtual void select(AMainCharacter* character) override;

	virtual void unselect(AMainCharacter* character) override;

	virtual void startRound() override;

	virtual void endRound() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void setNewWorkStation(AWorkstation* newWorkstation);

	UFUNCTION(BlueprintImplementableEvent)
	void resetWorkstation();

	UFUNCTION(BlueprintCallable)
	void updateWorkBehavior(UBehaviorTree* NewBehavior);

};
