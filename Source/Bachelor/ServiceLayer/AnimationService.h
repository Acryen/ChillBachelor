#pragma once

#include "CoreMinimal.h"
#include "AnimationService.generated.h"

class ACharacterBase;

UCLASS()
class BACHELOR_API UAnimationService : public UObject
{
	GENERATED_BODY()
	
public:
	/*
	 * Combat
	 */
	
	static void PlayLightAttackMontage(AActor* Actor);

	// Wird benutzt im EndOfattackNotifyState (in Begin)
	// Ggf. sollte diese function auch heavy attacks handeln und einen weiteren boolean Parameter (Light/Heavy) dafür erhalten.
	// Selbiges auch evtl. für die PlayLightAttackMontage
	static void EndOfLightAttackTriggered(AActor* Actor);

	// Wird benutzt im EndOfAttackNotifyState (in Tick)
	static void StopMontageOnVelocity(AActor* Actor);

	static void PlayBlockMontage(AActor* Actor);
	static void StopBlockMontage(AActor* Actor);

	static bool IsInMovementBlockingAnimation(AActor* Actor);

	/*
	 * Death
	 */
	
	static void PlayDeathMontage(AActor* Actor);
};
