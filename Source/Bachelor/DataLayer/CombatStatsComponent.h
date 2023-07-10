// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatStatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACHELOR_API UCombatStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeaponBase> WeaponBlueprint;

	UPROPERTY()
	AWeaponBase* Weapon;	
};
