// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMechanicsCharacter.h"
#include "GameMechanicsProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/Interactable_Interface.h"


//////////////////////////////////////////////////////////////////////////
// AGameMechanicsCharacter

AGameMechanicsCharacter::AGameMechanicsCharacter()
{
	// Character doesnt have a rifle at start

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AGameMechanicsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGameMechanicsCharacter::CheckForInteractables()
{
	FVector CameraLoc;
	FRotator CameraRot;

	GetActorEyesViewPoint(CameraLoc, CameraRot);

	FVector EndLoc = CameraLoc + (CameraRot.Vector() * MaxInteractDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLoc, EndLoc, ECC_Visibility, QueryParams);
	//DrawDebugLine(GetWorld(), CameraLoc, EndLoc, FColor::Blue, true, 1.0f, 0, 1.0f);

	ActorInRange = bHit ? HitResult.GetActor() : nullptr;
}

void AGameMechanicsCharacter::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,TEXT("Interacting"));
	if (ActorInRange != nullptr)
	{
		if (ActorInRange->GetClass()->ImplementsInterface(UInteractable_Interface::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,TEXT("Implement Interface"));
			Cast<IInteractable_Interface>(ActorInRange)->InteractPure(this);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,TEXT("Didnt Implement Interface"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange,TEXT("No Actor In Range"));
	}
}

void AGameMechanicsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForInteractables();
}

//////////////////////////////////////////////////////////////////////////// Input

void AGameMechanicsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &AGameMechanicsCharacter::Interact);


		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGameMechanicsCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGameMechanicsCharacter::Look);
	}
}


void AGameMechanicsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGameMechanicsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
