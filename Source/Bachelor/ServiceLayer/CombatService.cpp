#include "CombatService.h"

#include "AnimationService.h"
#include "BrainComponent.h"
#include "Bachelor/ActorLayer/Controller/AIController/EnemyAIController.h"
#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/CollectableStatsComponent.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

/*
 * Simple health functionality
 */

void UCombatService::ApplyDamage(AActor* Attacker, AActor* Victim)
{
	// Pointer / Conidition Check start
	
    if (!Attacker || !Victim) return;

	UHealthComponent* VictimHealthComp = Cast<UHealthComponent>(Victim->GetComponentByClass(UHealthComponent::StaticClass()));
	if (!VictimHealthComp || VictimHealthComp->bIsDead) return;

	UAnimationComponent* AttackerAnimationComp = Cast<UAnimationComponent>(Attacker->GetComponentByClass(UAnimationComponent::StaticClass()));
	UAnimationComponent* VictimAnimationComp = Cast<UAnimationComponent>(Victim->GetComponentByClass(UAnimationComponent::StaticClass()));
	if (!AttackerAnimationComp || !VictimAnimationComp || VictimAnimationComp->CombatState == ECombatState::Blocking) return;

	UCombatStatsComponent* CombatStatsComponent = Cast<UCombatStatsComponent>(Attacker->GetComponentByClass(UCombatStatsComponent::StaticClass()));
	if (!CombatStatsComponent || !CombatStatsComponent->Weapon) return;

	UWeaponStatsComponent* WeaponStatsComponents = CombatStatsComponent->Weapon->GetWeaponStatsComponent();
	if (!WeaponStatsComponents) return;

	// Functionality start

	int32 DamageToApply = WeaponStatsComponents->Damage * *AttackerAnimationComp->AttackChainDamageModifierMap.Find(AttackerAnimationComp->AttackCounter);

	VictimHealthComp->CurrentHealth -= DamageToApply;

	if (VictimHealthComp->CurrentHealth <= 0)
	{
		 ApplyDeath(Victim);
	}

	// TODO Debugmessage
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UCombatService::ApplyDamage - %i"), DamageToApply));
}

void UCombatService::ApplyStatIncrease(AActor* ItemCollected, AActor* Collector)
{
	if (!ItemCollected || !Collector) return;

	UCollectableStatsComponent* CollectableStatsComponent = Cast<UCollectableStatsComponent>(ItemCollected->GetComponentByClass(UCollectableStatsComponent::StaticClass()));
	if (!CollectableStatsComponent) return;

	UHealthComponent* HealthComponent = Cast<UHealthComponent>(Collector->GetComponentByClass(UHealthComponent::StaticClass()));

	switch (CollectableStatsComponent->CollectableType)
	{
	case ECollectableType::Heal:
		if (!HealthComponent) return;
		
		HealthComponent->CurrentHealth += CollectableStatsComponent->HealAmount;
		if (HealthComponent->CurrentHealth > HealthComponent->MaxHealth)
		{
			HealthComponent->CurrentHealth = HealthComponent->MaxHealth;
		}

		// TODO Debugmessage
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(
			TEXT("UCombatService::ApplyStatIncrease - New CurrentHealth %i"), HealthComponent->CurrentHealth));
		break;

	case ECollectableType::MaxHealth:
		if (!HealthComponent) return;
		
		HealthComponent->MaxHealth += CollectableStatsComponent->MaxHealthIncrease;
		HealthComponent->CurrentHealth += CollectableStatsComponent->MaxHealthIncrease;

		// TODO Debugmessage
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(
			TEXT("UCombatService::ApplyStatIncrease - New MaxHealth %i"), HealthComponent->MaxHealth));
		break;

	case ECollectableType::Damage:
		UCombatStatsComponent* CombatStatsComponent = Cast<UCombatStatsComponent>(Collector->GetComponentByClass(UCombatStatsComponent::StaticClass()));
		if (!CombatStatsComponent || !CombatStatsComponent->Weapon) return;
		
		UWeaponStatsComponent* WeaponStatsComponent = CombatStatsComponent->Weapon->GetWeaponStatsComponent();
		if (!WeaponStatsComponent) return;
			
		WeaponStatsComponent->Damage *= CollectableStatsComponent->DamageMultiplier;

		// TODO Debugmessage
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(
			TEXT("UCombatService::ApplyStatIncrease - New WeaponDamage %i"), WeaponStatsComponent->Damage));
		break;
	}
}

void UCombatService::ApplyDeath(AActor* Victim)
{
	if (!Victim) return;
	
	UHealthComponent* HealthComponent = Cast<UHealthComponent>(Victim->GetComponentByClass(UHealthComponent::StaticClass()));
	if (!HealthComponent) return;
	HealthComponent->bIsDead = true;

	APawn* VictimPawn = Cast<APawn>(Victim);
	if (!VictimPawn) return;
	
	AAIController* AIController = Cast<AAIController>(VictimPawn->GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("ApplyDeath"));
	}
	// The else block is switched to if the player dies instead of an opponent.
	// Then the boolean of the blackboard IsPlayerDead of each AI opponent should be set to true.
	else
	{
		TArray<AActor*> AIControllersArray;
		UGameplayStatics::GetAllActorsOfClass(VictimPawn, AAIController::StaticClass() , AIControllersArray);

		for (AActor* Actor : AIControllersArray)
		{
			AAIController* Controller = Cast<AAIController>(Actor);
			if (!Controller->GetBlackboardComponent()) return;
			Controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerDead"), true);
		}
	}

	UAnimationService::PlayDeathMontage(Victim);
}
