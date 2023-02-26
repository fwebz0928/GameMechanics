// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMECHANICS_API UMainPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class AGameMechanicsCharacter* PlayerCharacter;
};
