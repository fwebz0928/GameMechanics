// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthUpdated, float, UpdatedHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxHealthUpdated, float, UpdatedHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoHealth);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEMECHANICS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float CurrentHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	float MaxHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Health)
	bool bResetHealthOnBeginPlay;

public:
	//DELEGATES
	/**
	 * @brief Gets Called anytime the current health is updated either healed or damaged.
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HealthDelegate)
	FHealthUpdated OnHealthUpdated;
	/**
	 * @brief Gets Called when the current health reaches 0
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HealthDelegate)
	FNoHealth OnNoHealth;
	/**
	 * @brief Called whenever the max health is updated
	 */
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HealthDelegate)
	FMaxHealthUpdated OnMaxHealthUpdated;


	//GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Health)
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Health)
	float GetMaxHealth() const { return MaxHealth; }


	//CURRENT HEALTH UTIL FUNCTIONS
	/**
	 * @brief Gets the current health in decimal format 0-1. (*Good for Progress bars)
	 * @return Returns decimal health
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Health)
	float GetHealthDecimal() const { return CurrentHealth / MaxHealth; }

	/**
	 * @brief Damages the current health by a specific Amount
	 * @param Damage Amount to subtract from the current health
	 * @return returns the current health after being altered
	 */
	UFUNCTION(BlueprintCallable, Category = Health)
	float DamageHealth(float Damage);
	/**
	 * @brief Heal the current health by a specified amount
	 * @param Heal Amount to heal the current health by
	 * @return returns the current health after being altered
	 */
	UFUNCTION(BlueprintCallable, Category = Health)
	float Heal(float Heal);
	/**
	 * @brief Resets the current health back to the max health
	 */
	UFUNCTION(BlueprintCallable, Category = Health)
	void ResetHealth() { CurrentHealth = MaxHealth; };

	//MAX HEALTH
	UFUNCTION(BlueprintCallable, Category = Health)
	float IncreaseMaxHealth(float Amount);

	/**
	 * @brief Decreases the total max health by a specified amount
	 * @param Amount Amount to decrease the max Health by
	 * @return returns the altered health amount
	 */
	UFUNCTION(BlueprintCallable, Category = Health)
	float DecreaseMaxHealth(float Amount);

	


};
