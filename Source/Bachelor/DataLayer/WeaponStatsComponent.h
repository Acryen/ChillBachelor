// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACHELOR_API UWeaponStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponStatsComponent();

	/*
	 * Spawn variables
	 */

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector WeaponLocation = FVector(-6.f, 3.f, -4.f);

	UPROPERTY(EditAnywhere, Category = Weapon)
	FRotator WeaponRotation = FRotator(17.999950, 68.399808, 291.599164);

	UPROPERTY(EditAnywhere, Category = Weapon)
	FVector WeaponScale = FVector(1.3);

	/*
	 * Weapon stats
	 */

	UPROPERTY(EditAnywhere, Category = Damage)
	int32 Damage = 34;
};
