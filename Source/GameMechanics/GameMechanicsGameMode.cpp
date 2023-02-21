// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMechanicsGameMode.h"
#include "GameMechanicsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameMechanicsGameMode::AGameMechanicsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
