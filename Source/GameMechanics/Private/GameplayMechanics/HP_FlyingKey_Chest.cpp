// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMechanics/HP_FlyingKey_Chest.h"

// Sets default values
AHP_FlyingKey_Chest::AHP_FlyingKey_Chest()
{
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	RootComponent = ChestMesh;

	TriggerBox = CreateDefaultSubobject<UStaticMeshComponent>("TriggerBox");
	TriggerBox->SetupAttachment(RootComponent);


	//Create the Spring arm to be used in the actor
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;

	//Create and setup camera to be attached to the spring arm
	ChestCameraComponent = CreateDefaultSubobject<UCameraComponent>("ChestCamera");
	ChestCameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ChestCameraComponent->bUsePawnControlRotation = false;
}

void AHP_FlyingKey_Chest::OnInteracted_Implementation(AGameMechanicsCharacter* PCharacter)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,TEXT("BP Implementation "));
}

void AHP_FlyingKey_Chest::InteractPure(AGameMechanicsCharacter* PCharacter)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green,TEXT("Pure Implementation "));
}


//void AHP_FlyingKey_Chest::OnInteracted()
//{
//	IInteractable_Interface::OnInteracted();
//}

// Called when the game starts or when spawned
void AHP_FlyingKey_Chest::BeginPlay()
{
	Super::BeginPlay();
}
