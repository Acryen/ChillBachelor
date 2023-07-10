// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationComponent.h"


UAnimationComponent::UAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Die AnimationComponent soll von Beginn an default Standardwerte in der TMap für animation damage modifier haben
	int32 AnimationCounter = 1;
	float AnimationDamageModifier = 1.f;

	for (int32 i = 0; i < LightAttackChainLength; i++)
	{
		AttackChainDamageModifierMap.Add(AnimationCounter, AnimationDamageModifier);
		AnimationCounter++;
		AnimationDamageModifier += 0.5f;
	}
}
