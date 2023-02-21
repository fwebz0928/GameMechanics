// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMechanics/GameMechanicsCharacter.h"
#include "UObject/Interface.h"
#include "Interactable_Interface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable_Interface : public UInterface
{
	GENERATED_BODY()
};

class GAMEMECHANICS_API IInteractable_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interact")
	void OnInteracted(AGameMechanicsCharacter* PCharacter);

	UFUNCTION()
	virtual void InteractPure(AGameMechanicsCharacter* PCharacter) = 0;
};
