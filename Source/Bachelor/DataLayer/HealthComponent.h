// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BACHELOR_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

public:
	UPROPERTY(EditAnywhere, Category = Health)
	int32 MaxHealth = 100;
	int32 CurrentHealth = MaxHealth;
	
	bool bIsDead = false;
};
