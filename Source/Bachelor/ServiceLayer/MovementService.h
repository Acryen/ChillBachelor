// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ACharacterBase;
struct FInputActionValue;

class BACHELOR_API UMovementService
{
public:
	static void MoveByInputAxis(APawn* UserPawn, const FInputActionValue& Value);
	static void FloatUpDown(AActor* Actor, float InitialHeight, float Distance, float Speed, float ActorGameTimeSinceCreation);
};
