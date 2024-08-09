// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "../TwitchGameJame2024.h"
#include "Worker.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* pressedLClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RClickAction;

	/** Escape Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EscapeAction;

	/** Erase Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EraseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StartRoundAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> deleteBoxClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Workers, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> workerPlaceMarkerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Workers, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWorker> workerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trucks, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TruckActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	int32 gridSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = RoundTime, meta = (AllowPrivateAccess = "true"))
	float defaultRoundTime = 120;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = RoundTime, meta = (AllowPrivateAccess = "true"))
	float increaseTime = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = RoundTime, meta = (AllowPrivateAccess = "true"))
	int32 incrementToIncreaseRoundTime = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trucks, meta = (AllowPrivateAccess = "true"))
	int32 defaultNumTrucks = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trucks, meta = (AllowPrivateAccess = "true"))
	int32 increaseAmountMax = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Trucks, meta = (AllowPrivateAccess = "true"))
	int32 incrementToGiveAddTrucks = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Contracts, meta = (AllowPrivateAccess = "true"))
	int32 incrementToGiveContract = 3;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool inBuildMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool inBuildModeWorker;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool inEraseMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Management)
	bool inPathingMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	FBuildableStruct selectedBuildingInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool isInRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	FTimerHandle startRoundTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Building)
	float timeToCancel = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Building)
	int32 money = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	int32 moneyMadeInRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	int32 roundNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	float currentRoundTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	FTimerHandle roundTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	int32 numTrucks;

	


private:

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	ABuildablesBase* buildingHolo;

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	FRotator buildingRotation;

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	TArray<ABuildablesBase*> placedThisTurn;

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	ABuildablesBase* hoveredBuilding;

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	AActor* deleteBox;

	UPROPERTY(VisibleDefaultsOnly, Category = Building)
	AActor* WorkerPlaceMarker;

	IInteractInterface* HoveredActor;

	IInteractInterface* SelectedActor;

	int32 priceOfCurrentBuilding;

	FTimerHandle truckTimer;

	FTimerHandle addGarbadgeTimer;

	

public:
	AMainCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void LClick(const FInputActionValue& Value);

	void pressedLClick(const FInputActionValue& Value);

	void RClick(const FInputActionValue& Value);

	void escapePressed(const FInputActionValue& Value);

	void erasePressed(const FInputActionValue& Value);

	void startRoundPressed();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

public:
	virtual void Tick(float DeltaTime) override;

public:

	

	UFUNCTION(BlueprintCallable)
	void enterBuildMode(FBuildableStruct buildingInfo);

	UFUNCTION(BlueprintCallable)
	void enterBuildModeWorker(int32 price);
	
	UFUNCTION(BlueprintCallable)
	void spawnDropOffTruckTruck();

	UFUNCTION()
	void SpawnCollectionTruck();

	UFUNCTION()
	void StartRound();

	UFUNCTION()
	void EndRound();

	UFUNCTION(BlueprintImplementableEvent)
	void showEndScreen(bool giveContract);

	UFUNCTION()
	void endGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void playRoundMusic();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void playPreRoundMusic();



private:

	UFUNCTION()
	void spawnNewPlaceable();

	UFUNCTION()
	void destroyActor();

	UFUNCTION()
	void exitMode();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};