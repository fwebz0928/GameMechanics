// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMechanics/Components/HealthComponent.h"

//Note: Basic Health component system that can be added to any actor, character, pawn, or anything that can accept a actor component.


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bResetHealthOnBeginPlay = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bResetHealthOnBeginPlay)
		ResetHealth();
}


float UHealthComponent::DamageHealth(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	OnHealthUpdated.Broadcast(CurrentHealth);

	//Check to see if any health left
	if (CurrentHealth <= 0.0f)
		OnNoHealth.Broadcast();

	return CurrentHealth;
}

float UHealthComponent::Heal(const float Heal)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Heal, 0.0f, MaxHealth);
	OnHealthUpdated.Broadcast(CurrentHealth);
	return CurrentHealth;
}

float UHealthComponent::IncreaseMaxHealth(const float Amount)
{
	MaxHealth += Amount;
	OnMaxHealthUpdated.Broadcast(MaxHealth);
	return MaxHealth;
}

float UHealthComponent::DecreaseMaxHealth(const float Amount)
{
	MaxHealth = FMath::Clamp(MaxHealth - Amount, 0.0f, MaxHealth);
	OnMaxHealthUpdated.Broadcast(MaxHealth);
	return MaxHealth;
}
