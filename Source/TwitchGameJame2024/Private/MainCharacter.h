// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "../TwitchGameJame2024.h"
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
	UInputAction* RClickAction;

	/** Escape Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EscapeAction;

	/** Erase Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EraseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> deleteBoxClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	int32 gridSize;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool inBuildMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	bool inEraseMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Building)
	FBuildableStruct selectedBuildingInfo;

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


public:
	AMainCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void LClick(const FInputActionValue& Value);

	void RClick(const FInputActionValue& Value);

	void escapePressed(const FInputActionValue& Value);

	void erasePressed(const FInputActionValue& Value);

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

private:

	UFUNCTION()
	void spawnNewPlaceable();

	UFUNCTION()
	void destroyActor();


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};