// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyingKey.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMechanics/Widgets/HPFlyingKeyWidget.h"
#include "Interfaces/Interactable_Interface.h"
#include "HP_FlyingKey_Chest.generated.h"

UCLASS()
class GAMEMECHANICS_API AHP_FlyingKey_Chest : public APawn, public IInteractable_Interface
{
	GENERATED_BODY()

	//Chest meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ChestMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh_L;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh_R;

	//Chest utils meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* KeyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SchoolDiscMesh;

	//Triggers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TriggerBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SmackTrigger;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* KeyholeLocation;

	//Camera Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ChestCameraComponent;

public:
	// Sets default values for this actor's properties
	AHP_FlyingKey_Chest();

	void OnInteracted_Implementation(AGameMechanicsCharacter* PCharacter) override; // Include Blueprint Event
	virtual void InteractPure(AGameMechanicsCharacter* PCharacter) override; // C++ only

	// Called to bind functionality to input


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void KeyAtLocation(AFlyingKey* ChestOwnedKey);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SmackKey();

	UFUNCTION(BlueprintCallable)
	void ExitChest();

	UFUNCTION(BlueprintImplementableEvent)
	void MoveKeyToPositionOnSide();
	UFUNCTION(BlueprintImplementableEvent)
	void StopMovingKey();
	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoors();


	/**
	 * @brief Gets a point across from the key thru the zpne to the other side of the key area
	 * @return Returns a vector3 to a point across position
	 */
	FVector GetPointAcross();

	/**
	 * @brief Gets a random point in the side boxes for the key to move into a new position
	 * @return Returns a random vector 3
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetRandomPointInArea();
	/**
	 * @brief Checks if the actual key mesh is in the zone currently
	 * @param PrimitiveOverlap  The overlap primitive to check if its the key mesh
	 * @param bInZone to set if its in the zone or not on the overlap
	 */
	UFUNCTION(BlueprintCallable)
	void DetectKeyInZone(UPrimitiveComponent* PrimitiveOverlap, bool bInZone);
	/**
	 * @brief Smoothly rotates the door using a lerp and alpha value
	 * @param Value Value of the alpha for the lerp
	 */
	UFUNCTION(BlueprintCallable)
	void OpenDoorsTimeline(float Value) const;
	/**
	 * @brief Called when the doors are fully opened since the chest is fully done 
	 */
	UFUNCTION(BlueprintCallable)
	void ChestBeaten();



	void MoveKeyOnSide();
	void MoveKeyAcross() const;


	bool IsKeyOnLeftOfCenter() const;

	UFUNCTION(BlueprintCallable, Category = KeyMovement)
	void MovementTimelineCallback(float Value);



	//Movement
	/**
	 * @brief Size of the box that the random position can be inside 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=KeyMovement, meta=(AllowPrivateAccess = "true"))
	FVector SideBoxSize;
	/**
	 * @brief Position of the box compared to the arrow marking the center
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=KeyMovement, meta=(AllowPrivateAccess = "true"))
	FVector SideBoxOffset;
	/**
	 * @brief Rotation amount of the doors when they are opened
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=KeyMovement, meta=(AllowPrivateAccess = "true"))
	float DoorRotation;
	/**
	 * @brief Set when the key overlaps the smack trigger and unset when it leaves
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=KeyMovement, meta=(AllowPrivateAccess = "true"))
	bool bKeyInZone;
	/**
	 * @brief When the key is smacked used to stop the rest of the functions from it
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=KeyMovement, meta=(AllowPrivateAccess = "true"))
	bool bKeySmacked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Widget, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UHPFlyingKeyWidget> FlyingKeyWidgetClass;

private:
	bool bKeyAtLocation = false;

	UPROPERTY()
	AFlyingKey* FlyingKey;
	UPROPERTY()
	APlayerController* PController;
	UPROPERTY()
	AGameMechanicsCharacter* PlayerChar;
	UPROPERTY()
	UHPFlyingKeyWidget* FlyingKeyWidget;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return ChestCameraComponent; }
};
