// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayMechanics/HP_FlyingKey_Chest.h"
#include "EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

//TODO Make key float around when reach location and not interacted with
//TODO: lerp the camera views so its not a snap and is way smoother when interacting and possesing the chest
//TODO: Finish v1 then go back and make more performant 


//Note: In game when you complete the chest mini-game the doors open and your houses disc is inside
//Note: with your house logo there are many ways this can be done from swapping the material when choosing
//Note: the school , to just checking what school you are and enabling that one at runtime , spawning the specific school disc
//Note: for this example im just gunna set in deactivate then when complete activate the mesh

//Note: Currently using a simple keyboard event in the actual BP to handle calling the smack and exit since i cant figure out why the
//Note: Enhanced inputs are not currently being called even tho they are setup

//Note: Using static meshes for triggers to allow me to add a custom material to easily visualize the trigger areas vs the wire triggers

//Note: To move the key im gunna go simple. this isn't the best method but for simplicity sake gunna call the function from the timer and not have it loop
//Note: then ima call the timer from the function this way i can constantly have a random float be updated so it doesnt move on a timer and can feel more random

//Note: Since my modeling skills are not that good im using 3 separate meshes. one for the base , and the  2 doors
//Note: i will manually rotate the doors when unlocking the chest for the animation.
//Note: I will try to update this for v2 of this mechanic during the polish and optimization pass


// Sets default values
AHP_FlyingKey_Chest::AHP_FlyingKey_Chest()
{
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	RootComponent = ChestMesh;

	DoorMesh_L = CreateDefaultSubobject<UStaticMeshComponent>("Door_Left");
	DoorMesh_L->SetupAttachment(RootComponent);

	DoorMesh_R = CreateDefaultSubobject<UStaticMeshComponent>("Door_Right");
	DoorMesh_R->SetupAttachment(RootComponent);


	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>("KeyMesh");
	KeyMesh->SetupAttachment(RootComponent);
	KeyMesh->SetVisibility(false);

	SchoolDiscMesh = CreateDefaultSubobject<UStaticMeshComponent>("SchoolDisc");
	SchoolDiscMesh->SetupAttachment(RootComponent);

	//Trigger used when the moving key overlaps it
	TriggerBox = CreateDefaultSubobject<UStaticMeshComponent>("TriggerBox");
	TriggerBox->SetupAttachment(RootComponent);

	//Trigger used when player press button to smack key into the hole
	SmackTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SmackTrigger"));
	SmackTrigger->SetupAttachment(RootComponent);

	KeyholeLocation = CreateDefaultSubobject<UArrowComponent>("KeyHoleLocation");
	KeyholeLocation->SetupAttachment(RootComponent);


	//Create the Spring arm to be used in the actor
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;

	//Create and setup camera to be attached to the spring arm
	ChestCameraComponent = CreateDefaultSubobject<UCameraComponent>("ChestCamera");
	ChestCameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ChestCameraComponent->bUsePawnControlRotation = false;

	DrawDebugSolidBox(GetWorld(), KeyholeLocation->GetComponentLocation(), SideBoxSize, FColor::Cyan, false, 10.0f);
}


// Called when the game starts or when spawned
void AHP_FlyingKey_Chest::BeginPlay()
{
	Super::BeginPlay();

	PController = GetWorld()->GetFirstPlayerController();
	KeyMesh->SetVisibility(false, false);
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
	//	UEnhancedInputLocalPlayerSubsystem>(PController->GetLocalPlayer()))
	//{
	//	Subsystem->AddMappingContext(DefaultMappingContext, 0);
	//}
}

void AHP_FlyingKey_Chest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	//{
	//	EnhancedInputComponent->BindAction(SmackAction, ETriggerEvent::Triggered, this, &AHP_FlyingKey_Chest::SmackKey);
	//	EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Triggered, this, &AHP_FlyingKey_Chest::ExitChest);
	//}
}

//Interactions
void AHP_FlyingKey_Chest::OnInteracted_Implementation(AGameMechanicsCharacter* PCharacter)
{
}

void AHP_FlyingKey_Chest::InteractPure(AGameMechanicsCharacter* PCharacter)
{
	//if (!bKeyAtLocation) return;

	//Keep a ref to the player character so we can reposses it after we finish or leave
	PlayerChar = PCharacter;

	//Add the key widget to the player viewport
	FlyingKeyWidget = CreateWidget<UHPFlyingKeyWidget>(GetWorld(), FlyingKeyWidgetClass);
	if (FlyingKeyWidget != nullptr)
		FlyingKeyWidget->AddToViewport();

	//Destroy the movement key
	if (FlyingKey != nullptr)
		FlyingKey->Destroy();


	//	//Save the player character camera and change views to the chest view and enable its input
	if (PController != nullptr)
	{
		//Have the player controller posses this pawn 
		if (PController->GetPawn() != nullptr)
			PController->UnPossess();

		PController->Possess(this);
		EnableInput(PController);

		KeyMesh->SetVisibility(true);

		MoveKeyOnSide();
	}
}


//Input Functions
void AHP_FlyingKey_Chest::SmackKey()
{
	if (bKeyInZone == false) return;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta,TEXT("Smacking Key"));
	StopMovingKey();
	OpenDoors();
	KeyMesh->SetWorldLocation(KeyholeLocation->GetComponentLocation());
	bKeySmacked = true;
}

void AHP_FlyingKey_Chest::ExitChest()
{
	if (bKeySmacked == true || PController == nullptr) return;
	//Stop possessing this pawn and reposses the player character that we saved a ref to on interaction
	PController->UnPossess();
	PController->Possess(PlayerChar);

	//Remove the Key Widget
	FlyingKeyWidget->RemoveFromParent();
}


//Key Movement 
FVector AHP_FlyingKey_Chest::GetPointAcross()
{
	return FVector::One();
}

void AHP_FlyingKey_Chest::DetectKeyInZone(UPrimitiveComponent* PrimitiveOverlap, bool bInZone)
{
	if (PrimitiveOverlap != KeyMesh) return;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan,TEXT("KeyInZone"));
	bKeyInZone = bInZone;
}

void AHP_FlyingKey_Chest::MoveKeyOnSide()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,TEXT("Moving Key"));
	//This calls a blueprint Native event that has a timeline to lerp to a new position
	MoveKeyToPositionOnSide();
}

void AHP_FlyingKey_Chest::MoveKeyAcross() const
{
}

FVector AHP_FlyingKey_Chest::GetRandomPointInArea()
{
	FVector ArrowPos = KeyholeLocation->GetComponentLocation();
	FVector SidePos;

	if (IsKeyOnLeftOfCenter())
		SidePos = FVector(ArrowPos.X + SideBoxOffset.X, ArrowPos.Y + SideBoxOffset.Y,
		                  ArrowPos.Z + SideBoxOffset.Z);
	else
		SidePos = FVector(ArrowPos.X + -SideBoxOffset.X, ArrowPos.Y + -SideBoxOffset.Y,
		                  ArrowPos.Z + -SideBoxOffset.Z);

	FVector RandomPoint = FMath::RandPointInBox(FBox(SidePos - SideBoxSize, SidePos + SideBoxSize));

	FVector LocalPoint = GetRootComponent()->GetComponentTransform().
	                                         InverseTransformPosition(RandomPoint);

	DrawDebugBox(GetWorld(), SidePos, SideBoxSize, FColor::Cyan, false, 1.0f);
	DrawDebugPoint(GetWorld(), LocalPoint, 20.0f, FColor::Black, false, 1.0f);

	return LocalPoint;
}

bool AHP_FlyingKey_Chest::IsKeyOnLeftOfCenter() const
{
	FVector ArrowForwardVector = KeyholeLocation->GetForwardVector();
	FVector ArrowLocation = KeyholeLocation->GetComponentLocation();

	FVector KeyMeshLocation = KeyMesh->GetComponentLocation();

	FVector VectorToTest = KeyMeshLocation - ArrowLocation;

	FVector ProjectedVector = VectorToTest - FVector::DotProduct(VectorToTest, ArrowForwardVector) * ArrowForwardVector;
	float CrossProduct = FVector::CrossProduct(ArrowForwardVector, ProjectedVector).X;

	return (CrossProduct > 0.0f);
}

void AHP_FlyingKey_Chest::OpenDoorsTimeline(const float Value) const
{
	//Rotate Right Door
	FRotator RightDoorRotator = FRotator(0.0f, -DoorRotation, 0.0f);
	DoorMesh_R->SetRelativeRotation(FMath::Lerp
		(
			DoorMesh_R->GetRelativeRotation(),
			RightDoorRotator,
			Value
		));
	//Rotate Left Door
	FRotator LeftDoorRotator = FRotator(0.0f, DoorRotation, 0.0f);
	DoorMesh_L->SetRelativeRotation(FMath::Lerp
		(
			DoorMesh_L->GetRelativeRotation(),
			LeftDoorRotator,
			Value
		));
}

void AHP_FlyingKey_Chest::ChestBeaten()
{
	//Stop possessing this pawn and reposses the player character that we saved a ref to on interaction
	PController->UnPossess();
	PController->Possess(PlayerChar);

	//Remove the Key Widget
	FlyingKeyWidget->RemoveFromParent();
}

//Callbacks for the Timeline
void AHP_FlyingKey_Chest::MovementTimelineCallback(float Value)
{
	//If we successfully smacked the key return out so we dont keep moving the key around
	if (bKeySmacked) return;
	MoveKeyToPositionOnSide();
}


//When the Flying key is actually at the chest in the world
void AHP_FlyingKey_Chest::KeyAtLocation(AFlyingKey* ChestOwnedKey)
{
	bKeyAtLocation = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,TEXT("Key At Chest"));

	FlyingKey = ChestOwnedKey;
	//Enable the key that will be used to smack at the center to get the school disc
}
