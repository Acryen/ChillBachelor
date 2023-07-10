// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAttackChainTest.h"

#include "Bachelor/ActorLayer/Character/CharacterBase.h"
#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "Bachelor/ServiceLayer/AnimationService.h"


ALightAttackChainTest::ALightAttackChainTest()
{
	TimeLimit = 3.f;
}

bool ALightAttackChainTest::ArePointersValid()
{
	if (!Attacker ||
		!Attacker->GetCombatStatsComponent()->Weapon ||
		!Attacker->GetCombatStatsComponent()->Weapon->GetWeaponStatsComponent() ||
		!Attacker->GetAnimationComponent() ||
		!Victim ||
		!Victim->GetHealthComponent())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ALightAttackChainTest::StartTest()
{
	Super::StartTest();

	//
	// GIVEN
	
	if (!ArePointersValid())
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("Atleast one pointer in this test is null"));
		return;
	}

	float Distance = (Victim->GetActorLocation() - Attacker->GetActorLocation()).Length();
	if (Distance > AttackerVictimDistance)
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("Distance between Attacker and Victim is too big"));
		return;
	}

	if (AttackInChainToCheck > Attacker->GetAnimationComponent()->LightAttackChainLength)
	{
		FinishTest(EFunctionalTestResult::Error, TEXT("AttackInChainToCheck is greater than the Length of the actual attack chain"));
		return;
	}
	
	//
	// WHEN

	UAnimationService::PlayLightAttackMontage(Attacker);

	// Der Timer ist für den Fall, das spätere Angriffe getestet werden und soll diese auslösen
	FTimerHandle TimerHandleOne;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleOne, [this]()
	{
		UAnimationService::PlayLightAttackMontage(Attacker);
	}, 0.4f, true);	

	//
	// THEN

	// Handled in Tick
	
}

void ALightAttackChainTest::Tick(float DeltaSeconds)
{
	//
	// THEN
	
	Super::Tick(DeltaSeconds);

	if (!ArePointersValid()) return;

	int32 ExpectedDamageToApply = 0;

	if (AttackInChainToCheck <= Attacker->GetAnimationComponent()->LightAttackChainLength)
	{
		for (int32 i = 1; i <= AttackInChainToCheck; i++)
		{
			ExpectedDamageToApply += Attacker->GetCombatStatsComponent()->Weapon->GetWeaponStatsComponent()->Damage * *Attacker->GetAnimationComponent()->AttackChainDamageModifierMap.Find(i);
		}
	}
	
	int32 ExpectedHealth = Victim->GetHealthComponent()->MaxHealth - ExpectedDamageToApply;
	
	if (Victim->GetHealthComponent()->CurrentHealth == ExpectedHealth)
	{
		FinishTest(EFunctionalTestResult::Succeeded, FString::Printf(TEXT("Light attack chain of %i damage successfully applied"), AttackInChainToCheck));
	}
}
