#include "AnimationService.h"

#include "Bachelor/ActorLayer/Character/CharacterBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Components/CapsuleComponent.h"

void UAnimationService::PlayLightAttackMontage(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);	
	UAnimationComponent* AnimationComponent = Character->GetAnimationComponent();
	if (!Character || !AnimationComponent) return;

	// Wenn wir bereits am Angreifen sein sollten, soll nur der Buffer auf "ForAttack" gesestzt werden.
	// EndOfLightAttackTriggered() kümmert sich dann mit aktiven Buffer um das automatische Auslösen desn nächsten Angriffs.
	if (AnimationComponent->CombatState == ECombatState::Attacking)
	{
		AnimationComponent->CombatBufferState = ECombatBufferState::ForAttack;
		return;
	}

	AnimationComponent->CombatState = ECombatState::Attacking;
	
	switch (AnimationComponent->AttackCounter)
	{
	case 1:
		Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimationComponent->LightAttackOneMontage);
		break;
	case 2:
		Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimationComponent->LightAttackTwoMontage);
		break;
	case 3:
		Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimationComponent->LightAttackThreeMontage);
		break;
	default:
		break;
	}
}

void UAnimationService::EndOfLightAttackTriggered(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);	
	UAnimationComponent* AnimationComponent = Character->GetAnimationComponent();
	if (!Character || !AnimationComponent) return;
	
	AnimationComponent->CombatState = ECombatState::Idle;

	if (AnimationComponent->CombatBufferState == ECombatBufferState::ForAttack && AnimationComponent->AttackCounter < AnimationComponent->LightAttackChainLength)
	{
		AnimationComponent->AttackCounter++;
		AnimationComponent->CombatBufferState = ECombatBufferState::None;
		PlayLightAttackMontage(Character);
	}
	else if (AnimationComponent->CombatBufferState == ECombatBufferState::ForBlocking)
	{
		AnimationComponent->AttackCounter = 1;
		AnimationComponent->CombatBufferState = ECombatBufferState::None;
		PlayBlockMontage(Character);
	}
	else // Hier gehts rein, wenn entweder nicht weiter für den nächsten Angriff geklickt wurde oder das Ende der AttackCHain erreicht wurde
	{
		AnimationComponent->AttackCounter = 1;
		AnimationComponent->CombatBufferState = ECombatBufferState::None;
	}
}

void UAnimationService::StopMontageOnVelocity(AActor* Actor)
{
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (!Character) return;
	
	if (!Character->GetVelocity().IsNearlyZero())
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
	}
}

void UAnimationService::PlayBlockMontage(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);
	UAnimationComponent* AnimationComponent = Character->GetAnimationComponent();
	if (!Character || !AnimationComponent) return;

	// Aus dem Angriff heraus soll nicht geblockt werden können, aber gebuffert werden können.
	if (AnimationComponent->CombatState == ECombatState::Attacking)
	{
		AnimationComponent->CombatBufferState = ECombatBufferState::ForBlocking;
		return;
	}

	AnimationComponent->CombatState = ECombatState::Blocking;

	Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimationComponent->BlockMontage);
}

void UAnimationService::StopBlockMontage(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);
	UAnimationComponent* AnimationComponent = Character->GetAnimationComponent();
	if (!Character || !AnimationComponent) return;

	AnimationComponent->CombatState = ECombatState::Idle;

	Character->GetMesh()->GetAnimInstance()->Montage_Stop(AnimationComponent->BlockBlendOutTime, AnimationComponent->BlockMontage);
}

bool UAnimationService::IsInMovementBlockingAnimation(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);
	if (!Character) return false;

	UAnimationComponent* AnimationComponent = Character->GetAnimationComponent();
	UHealthComponent* HealthComponent = Character->GetHealthComponent();
	if (!AnimationComponent || !HealthComponent) return false;

	return AnimationComponent->CombatState != ECombatState::Idle || HealthComponent->bIsDead;
}

void UAnimationService::PlayDeathMontage(AActor* Actor)
{
	ACharacterBase* Character = Cast<ACharacterBase>(Actor);
	if (!Character) return;	

	float MontageDuration = Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetAnimationComponent()->DeathMontage);

	// TODO
	// Topic für Nick : https://forums.unrealengine.com/t/animation-montages-on-interrupt-on-blend-out-on-completed-in-c/123307/2
	// "Montage_SetBlendingOutDelegate" ist auf der Seite nicht beschrieben.
	// PlayMontageCallbackProxy.h
	//
	// FOnMontageBlendingOutStarted BlendingOutStartedDelegate;
	// BlendingOutStartedDelegate.BindStatic(&UAnimationService::OnMontageBlendingOut);

	if (!Character->GetWorld()) return;

	FTimerHandle TimerHandleOne;
	Character->GetWorld()->GetTimerManager().SetTimer(TimerHandleOne, [Character]()
	{
		Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		// Basically stop das Mesh daran das AnimationBlueprint zu nutzen
		Character->GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		
		Character->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Character->GetMesh()->SetSimulatePhysics(true);
	},
	MontageDuration - 0.25f, // 0.25f ist die BlendOutDuration in der AnimMontage
	false);
}
