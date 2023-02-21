// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMechanics/MothPainting.h"

// Sets default values
AMothPainting::AMothPainting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMothPainting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMothPainting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

