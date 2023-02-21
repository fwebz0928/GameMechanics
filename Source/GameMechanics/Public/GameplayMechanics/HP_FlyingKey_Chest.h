// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyingKey.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Interactable_Interface.h"
#include "HP_FlyingKey_Chest.generated.h"

UCLASS()
class GAMEMECHANICS_API AHP_FlyingKey_Chest : public AActor , public IInteractable_Interface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChestMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ChestCameraComponent;

public:
	// Sets default values for this actor's properties
	AHP_FlyingKey_Chest();

	void OnInteracted_Implementation(AGameMechanicsCharacter* PCharacter) override; // Include Blueprint Event
	virtual void InteractPure(AGameMechanicsCharacter* PCharacter) override; // C++ only

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFlyingKey* FlyingKey;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return ChestCameraComponent; }
};
