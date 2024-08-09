// Fill out your copyright notice in the Description page of Project Settings.


#include "GarbadgeTruck.h"

// Sets default values
AGarbadgeTruck::AGarbadgeTruck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGarbadgeTruck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGarbadgeTruck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

