// Fill out your copyright notice in the Description page of Project Settings.


#include "EndOfAttackNotifyState.h"

#include "Bachelor/ActorLayer/Character/CharacterBase.h"
#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/ServiceLayer/AnimationService.h"

void UEndOfAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;
	
	ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner());
	if (!Character) return;

	UAnimationService::EndOfLightAttackTriggered(Character);

	AWeaponBase* Weapon = Character->GetCombatStatsComponent()->Weapon;
	if (Weapon)
	{
		Weapon->EmptyHitBlacklistArray();
	}
}

void UEndOfAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp) return;
	UAnimationService::StopMontageOnVelocity(MeshComp->GetOwner());
}
