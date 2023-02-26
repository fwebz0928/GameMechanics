// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMechanics/FlyingKey.h"

#include "GameFramework/PawnMovementComponent.h"


// Sets default values
AFlyingKey::AFlyingKey()
{
	RootScene = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = RootScene;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>("KeyMesh");
	KeyMesh->SetupAttachment(RootComponent);

	OverlapComponent = CreateDefaultSubobject<USphereComponent>("Overlap Component");
	OverlapComponent->SetupAttachment(KeyMesh);

	MovementSpline = CreateDefaultSubobject<USplineComponent>("MovementSpline");
	MovementSpline->SetupAttachment(RootComponent);


	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFlyingKey::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyingKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanMove)
	{
		float DistanceToMove = MovementSpeed * DeltaTime;
		float SplineLength = MovementSpline->GetSplineLength();

		DistanceAlongSpline += DistanceToMove;

		// If we've gone past the end of the spline, stop moving the key
		if (DistanceAlongSpline >= SplineLength)
		{
			DistanceAlongSpline = SplineLength;
			bCanMove = false;
		}

		// Get the current location and the next location along the spline
		FVector CurrentLocation = MovementSpline->GetLocationAtDistanceAlongSpline(
			DistanceAlongSpline, ESplineCoordinateSpace::World);
		FVector NextLocation = MovementSpline->GetLocationAtDistanceAlongSpline(
			DistanceAlongSpline + DistanceToMove, ESplineCoordinateSpace::World);

		// Calculate the forward vector based on the current and next location
		FVector ForwardVector = (NextLocation - CurrentLocation).GetSafeNormal();

		// Calculate the rotation based on the forward vector
		FRotator Rotation = ForwardVector.Rotation();

		// Set the location and rotation of the mesh component
		KeyMesh->SetWorldLocationAndRotation(CurrentLocation, Rotation);
	}
}


void AFlyingKey::GoToChestLocation()
{
	//check if we can overlap so not constantly calling overlap calls if player is getting near the key while it moves
	if (bCanOverlap == false) return;
	bCanMove = true;
	bCanOverlap = false;
	OverlapComponent->SetActive(false);
}
