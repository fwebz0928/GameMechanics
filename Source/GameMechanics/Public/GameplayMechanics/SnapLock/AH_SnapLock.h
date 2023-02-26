// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AH_SnapLock.generated.h"

UCLASS()
class GAMEMECHANICS_API AAH_SnapLock : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SnapLockMesh;

public:
	// Sets default values for this actor's properties
	AAH_SnapLock();

protected:


public:
};
