// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimationComponent.generated.h"

UENUM()
enum class ECombatState : uint8
{
	Attacking UMETA(DisplayName = "Attacking"),
	Blocking UMETA(DisplayName = "Blocking"),
	Idle UMETA(DisplayName = "Idle")
};

UENUM()
enum class ECombatBufferState: uint8
{
	ForAttack UMETA(DisplayName = "ForAttacking"),
	ForBlocking UMETA(DisplayName = "ForBlocking"),
	None UMETA(DisplayName = "None")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACHELOR_API UAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAnimationComponent();

	/*
	 * Combat Montages
	 */

	ECombatState CombatState = ECombatState::Idle;
	ECombatBufferState CombatBufferState = ECombatBufferState::None;

	//
	// Combat Montages - Light Attack

	// TODO Ein Array aus structs die UAnimMontage und den jeweiligen DamageModifier beinhalten
	UPROPERTY(EditAnywhere, Category = LightAttack)
	class UAnimMontage* LightAttackOneMontage;

	UPROPERTY(EditAnywhere, Category = LightAttack)
	UAnimMontage* LightAttackTwoMontage;

	UPROPERTY(EditAnywhere, Category = LightAttack)
	UAnimMontage* LightAttackThreeMontage;

	UPROPERTY(EditAnywhere, Category = LightAttack)
	int32 LightAttackChainLength = 3;

	UPROPERTY(EditAnywhere, Category = LightAttack)
	TMap<int32, float> AttackChainDamageModifierMap;
	
	int32 AttackCounter = 1;

	//
	// Combat Montages - Heavy Attack

	UPROPERTY(EditAnywhere, Category = HeavyAttack)
	UAnimMontage* HeavyAttackMontage;

	//
	// Combat Montages - Block

	UPROPERTY(EditAnywhere, Category = Block)
	UAnimMontage* BlockMontage;
	
	UPROPERTY(EditAnywhere, Category = Block)
	float BlockBlendOutTime = 0.25f;

	/*
	 * Death Montages
	 */

	UPROPERTY(EditAnywhere, Category = Death)
	UAnimMontage* DeathMontage;
};
