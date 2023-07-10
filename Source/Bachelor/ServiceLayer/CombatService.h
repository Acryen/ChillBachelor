#pragma once

#include "CoreMinimal.h"

class ACharacterBase;
class ACollectable;

class BACHELOR_API UCombatService
{
public:
	static void ApplyDamage(AActor* Attacker, AActor* Victim);
	static void ApplyStatIncrease(AActor* ItemCollected, AActor* Collector);
	static void ApplyDeath(AActor* Victim);
};
