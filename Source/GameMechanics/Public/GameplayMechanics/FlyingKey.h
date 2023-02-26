// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "FlyingKey.generated.h"

UCLASS()
class GAMEMECHANICS_API AFlyingKey : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* KeyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* OverlapComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USplineComponent* MovementSpline;

public:
	// Sets default values for this actor's properties
	AFlyingKey();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest")
	class AHP_FlyingKey_Chest* OwningChest;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	void GoToChestLocation();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chest")
	float MovementSpeed = 1000.0f;

private:
	bool bCanMove = false;
	bool bCanOverlap = true;
	float DistanceAlongSpline;
};
