// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "LightAttackChainTest.generated.h"

UCLASS()
class BACHELOREDITOR_API ALightAttackChainTest : public AFunctionalTest
{
	GENERATED_BODY()

public:
	ALightAttackChainTest();
	
protected:
	bool ArePointersValid();
	virtual void StartTest() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, Category = Damage)
	class ACharacterBase* Attacker;

	UPROPERTY(EditAnywhere, Category = Damage)
	ACharacterBase* Victim;

	UPROPERTY(EditAnywhere, Category = Setup)
	int32 AttackInChainToCheck = 1;
	
	UPROPERTY(EditAnywhere, Category = Setup)
	float AttackerVictimDistance = 200.f;
};
