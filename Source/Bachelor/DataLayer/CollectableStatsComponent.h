// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectableStatsComponent.generated.h"

UENUM()
enum class ECollectableType : uint8
{
	// TODO Aus jedem Typen soll eine eigene Component werden
	Heal UMETA(DisplayName = "Heal"),
	MaxHealth UMETA(DisplayName = "MaxHealth"),
	Damage UMETA(DisplayName = "DamageIncrease")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACHELOR_API UCollectableStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCollectableStatsComponent();

protected:
	virtual void BeginPlay() override;

public:

	/*
	 * Stat Increase
	 */

	UPROPERTY(EditAnywhere, Category = Stats)
	ECollectableType CollectableType = ECollectableType::Heal;

	UPROPERTY(EditAnywhere, Category = Stats)
	int32 HealAmount = 25;

	UPROPERTY(EditAnywhere, Category = Stats)
	int32 MaxHealthIncrease = 25;

	UPROPERTY(EditAnywhere, Category = Stats)
	float DamageMultiplier = 1.1f;

	/*
	 * Floating
	 */

	// Diese Variable wird in BeginPlay() dieser Component gesetzt.
	float InitialHeight;

	UPROPERTY(EditAnywhere, Category = Floating)
	float FloatingDistance = 50.f;

	UPROPERTY(EditAnywhere, Category = Floating)
	float FloatingSpeed = 2.f;
};
