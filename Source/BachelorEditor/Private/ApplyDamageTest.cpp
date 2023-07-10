#include "ApplyDamageTest.h"

#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "Bachelor/ServiceLayer/CombatService.h"


AApplyDamageTest::AApplyDamageTest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AApplyDamageTest::StartTest()
{
	// Asserts functions are not being used here, because they fire off regardless of the pointer to check being null or valid.
	
	Super::StartTest();

	/*
	 * GIVEN
	 */

	//
	// Build Attacker
	
	AActor* Attacker = GetWorld()->SpawnActor<AActor>();
	if (Attacker == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Attacker is null."));
	}
	
	UAnimationComponent* AttackerAnimationComponent = Cast<UAnimationComponent>(Attacker->AddComponentByClass(UAnimationComponent::StaticClass(),
		false, FTransform::Identity, false));
	UCombatStatsComponent* AttackerCombatStatsComponent = Cast<UCombatStatsComponent>(Attacker->AddComponentByClass(UCombatStatsComponent::StaticClass(),
		false, FTransform::Identity, false));
	if (AttackerAnimationComponent == nullptr || AttackerCombatStatsComponent == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Attackers components are null."));
	}

	//
	// Build Attackers Weapon

	AWeaponBase* AttackersWeapon = GetWorld()->SpawnActor<AWeaponBase>();
	if (AttackersWeapon == nullptr || AttackersWeapon->GetWeaponStatsComponent() == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Attackers weapon is null."));
	}

	AttackerCombatStatsComponent->Weapon = AttackersWeapon;

	//
	// Build Victim

	AActor* Victim = GetWorld()->SpawnActor(AActor::StaticClass());
	if (Victim == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Victim is null."));
	}
	
	UHealthComponent* VictimHealthComponent = Cast<UHealthComponent>(Victim->AddComponentByClass(UHealthComponent::StaticClass(), false, FTransform::Identity, false));
	UAnimationComponent* VictimAnimationComponent = Cast<UAnimationComponent>(Victim->AddComponentByClass(UAnimationComponent::StaticClass(),
		false, FTransform::Identity, false));
	if (VictimHealthComponent == nullptr || VictimAnimationComponent == nullptr)
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Victims components are null."));
	}

	/*
	 * WHEN
	 */
	
	UCombatService::ApplyDamage(Attacker, Victim);

	/*
	 * THEN
	 */

	int32 ExpectedDamage = 34; // FrostWeaponDamage * AttackChainModifier(Attack 1 = 100% Damage)
	int32 ExpectedHealth = 100 - ExpectedDamage;

	FinishTest(VictimHealthComponent->CurrentHealth == ExpectedHealth ? EFunctionalTestResult::Succeeded : EFunctionalTestResult::Failed, FString(TEXT("Test finished")));
}