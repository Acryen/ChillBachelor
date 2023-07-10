// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class BACHELOR_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	virtual void BeginPlay() override;
	
	/*
	 * Components
	 */

	//
	// Components - DataLayer

	UPROPERTY(VisibleAnywhere, Category = Animation)
	class UAnimationComponent* AnimationComponent;

	UPROPERTY(VisibleAnywhere, Category = Health)
	class UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = CombatStats)
	class UCombatStatsComponent* CombatStatsComponent;
	
public:
	/*
	 * Getters & Setters
	 */

	//
	// Getters - Components

	FORCEINLINE UAnimationComponent* GetAnimationComponent() { return AnimationComponent; }
	FORCEINLINE UHealthComponent* GetHealthComponent() { return HealthComponent; }
	FORCEINLINE UCombatStatsComponent* GetCombatStatsComponent () { return CombatStatsComponent; }

};
