#include "AIController.h"
#include "BrainComponent.h"
#include "Animation/AnimInstance.h"
#include "Bachelor/ActorLayer/Actor/Collectable.h"
#include "Bachelor/ActorLayer/Character/EnemyCharacter.h"
#include "Bachelor/ActorLayer/Character/PlayerCharacter.h"
#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/CollectableStatsComponent.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "Bachelor/ServiceLayer/CombatService.h"
#include "Bachelor/ServiceLayer/MovementService.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/AutomationEditorCommon.h"

// TODO Registering UActorComponents does not work inside of simple automation tests, since Actor->GetWorld() always returns nullptr, but a valid pointer is required
// TODO by the AddComponent function to register the component.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFirstLightAttackFrostNew, "Custom.NotWorking.Combat.Damage.LightAttack1.FrostWeapon", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FFirstLightAttackFrostNew::RunTest(const FString& Parameters)
{
	return true;
	
	// GIVEN
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);
	
	AActor* Attacker = NewObject<AActor>(World, AActor::StaticClass());
	AActor* Victim = NewObject<AActor>(World, AActor::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);	

	UAnimationComponent* AnimationComponent = NewObject<UAnimationComponent>(Attacker, UAnimationComponent::StaticClass());
	TestNotNull(TEXT("AnimationComponent"), AnimationComponent);
	Attacker->AddComponent(TEXT("AnimationComponent"), false, FTransform::Identity, AnimationComponent, true);

	UHealthComponent* HealthComponent = NewObject<UHealthComponent>(Victim, UHealthComponent::StaticClass());
	TestNotNull(TEXT("HealthComponent"), HealthComponent);
	HealthComponent->RegisterComponent();

	// WHEN
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	int32 ExpectedDamage = 34; // FrostWeaponDamage * AttackChainModifier(Attack 1 = 100% Damage)
	int32 ExpectedHealth = 100 - ExpectedDamage;

	return TestEqual(TEXT("CurrentHealth"), HealthComponent->CurrentHealth, ExpectedHealth);
}

// TODO Even though Actor->GetOuter() returns the level, Actor->GetWorld() does not.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWorldTest, "Custom.NotWorking.World", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FWorldTest::RunTest(const FString& Parameters)
{
	return true;
	
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);
	
	AActor* Actor = NewObject<AActor>(World, AActor::StaticClass());
	TestNotNull(TEXT("Actor"), Actor);
	
	World->PersistentLevel->Actors.Add(Actor);
	
	UE_LOG(LogTemp, Warning, TEXT("Name is %s"), *Actor->GetOuter()->GetName());
	
	return TestNotNull(TEXT("Actors World"), Actor->GetOuter());
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFirstLightAttackFrost, "Custom.Combat.Damage.LightAttack1.FrostWeapon", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FFirstLightAttackFrost::RunTest(const FString& Parameters)
{
	// GIVEN
	ACharacterBase* Attacker = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	ACharacterBase* Victim = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers AnimationComponent"), Attacker->GetAnimationComponent());
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims Healthcomponent"), Victim->GetHealthComponent());

	Attacker->GetAnimationComponent()->AttackCounter = 1;
	
	AWeaponBase* Weapon = NewObject<AWeaponBase>(GetTransientPackage(), AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	// WHEN
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	int32 ExpectedDamage = 34; // FrostWeaponDamage * AttackChainModifier(Attack 1 = 100% Damage)
	int32 ExpectedHealth = 100 - ExpectedDamage;
	
	return TestEqual(TEXT("CurrentHealth"), Victim->GetHealthComponent()->CurrentHealth, ExpectedHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSecondLightAttackFrost, "Custom.Combat.Damage.LightAttack2.FrostWeapon", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSecondLightAttackFrost::RunTest(const FString& Parameters)
{
	// GIVEN
	ACharacterBase* Attacker = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	ACharacterBase* Victim = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers AnimationComponent"), Attacker->GetAnimationComponent());
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims HealthComponent"), Victim->GetHealthComponent());

	Attacker->GetAnimationComponent()->AttackCounter = 2;

	AWeaponBase* Weapon = NewObject<AWeaponBase>(GetTransientPackage(), AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	// WHEN
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	int32 ExpectedDamage = 34 * 1.5f; // FrostWeaponDamage * AttackChainModifier(Attack 2 = 150% Damage)
	int32 ExpectedHealth = 100 - ExpectedDamage;
	
	return TestEqual(TEXT("CurrentHealth"), Victim->GetHealthComponent()->CurrentHealth, ExpectedHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FThirdLightAttackFrost, "Custom.Combat.Damage.LightAttack3.FrostWeapon", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FThirdLightAttackFrost::RunTest(const FString& Parameters)
{
	// GIVEN
	ACharacterBase* Attacker = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	ACharacterBase* Victim = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers AnimationComponent"), Attacker->GetAnimationComponent());
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims HealthComponent"), Victim->GetHealthComponent());

	Attacker->GetAnimationComponent()->AttackCounter = 3;

	AWeaponBase* Weapon = NewObject<AWeaponBase>(GetTransientPackage(), AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	// WHEN
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	int32 ExpectedDamage = 34 * 2; // FrostWeaponDamage * AttackChainModifier(Attack 3 = 200% Damage)
	int32 ExpectedHealth = 100 - ExpectedDamage;
	
	return TestEqual(TEXT("CurrentHealth"), Victim->GetHealthComponent()->CurrentHealth, ExpectedHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlock, "Custom.Combat.Damage.Block", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlock::RunTest(const FString& Parameters)
{
	// GIVEN
	ACharacterBase* Attacker = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	ACharacterBase* Victim = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers AnimationComponent"), Attacker->GetAnimationComponent());
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims AnimationComponent"), Victim->GetAnimationComponent());
	TestNotNull(TEXT("Victims HealthComponent"), Victim->GetHealthComponent());

	Attacker->GetAnimationComponent()->AttackCounter = 1;

	AWeaponBase* Weapon = NewObject<AWeaponBase>(GetTransientPackage(), AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	Victim->GetAnimationComponent()->CombatState = ECombatState::Blocking;

	// WHEN
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	return TestEqual(TEXT("CurrentHealth"), Victim->GetHealthComponent()->CurrentHealth, Victim->GetHealthComponent()->MaxHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsDead, "Custom.Combat.Damage.Death.IsDead", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FIsDead::RunTest(const FString& Parameters)
{
	// GIVEN
	// Eine World wird benötigt, weil ansonsten Warnings entstehen, die den Test verlangsamen
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);
	
	ACharacterBase* Attacker = World->SpawnActor<ACharacterBase>(ACharacterBase::StaticClass());
	ACharacterBase* Victim = World->SpawnActor<ACharacterBase>(ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers AnimationComponent"), Attacker->GetAnimationComponent());
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims HealthComponent"), Victim->GetHealthComponent());

	AWeaponBase* Weapon = World->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	Attacker->GetAnimationComponent()->AttackCounter = 3;

	// WHEN
	// With given setup, 2 Attacks will kill
	UCombatService::ApplyDamage(Attacker, Victim);
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	return TestEqual(TEXT("bIsDead"), Victim->GetHealthComponent()->bIsDead, true);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNotIsDead, "Custom.Combat.Damage.Death.NotIsDead", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FNotIsDead::RunTest(const FString& Parameters)
{
	// GIVEN
	// Eine World wirt benötigt, weil ansonsten Warnings entstehen, die den Test verlangsamen
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);
	
	ACharacterBase* Attacker = World->SpawnActor<ACharacterBase>(ACharacterBase::StaticClass());
	ACharacterBase* Victim = World->SpawnActor<ACharacterBase>(ACharacterBase::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());
	TestNotNull(TEXT("Victims HealthComponent"), Victim->GetHealthComponent());

	AWeaponBase* Weapon = World->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	// WHEN
	// With given setup 1 Attack will not kill
	UCombatService::ApplyDamage(Attacker, Victim);

	// THEN
	return TestEqual(TEXT("bIsDead"), Victim->GetHealthComponent()->bIsDead, false);
}

// TODO The BrainComponent is the one that executes the logic of the BehaviorTree and needs to be queried. However, no BrainComponent is supplied with the spawn
// TODO of an AIController. CreateDefautlSubobject crashes the engine because no initializer can be found. 
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAIStopsLogicOnDeath, "Custom.NotWorking.Combat.Damage.Death.AIStopsLogicOnDeath", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAIStopsLogicOnDeath::RunTest(const FString& Parameters)
{
	return true;
	
	// GIVEN
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);
	
	ACharacterBase* Attacker = World->SpawnActor<ACharacterBase>(ACharacterBase::StaticClass());
	AEnemyCharacter* Victim = World->SpawnActor<AEnemyCharacter>(AEnemyCharacter::StaticClass());
	AAIController* AIController = World->SpawnActor<AAIController>(AAIController::StaticClass());
	TestNotNull(TEXT("Attacker"), Attacker);
	TestNotNull(TEXT("Victim"), Victim);
	TestNotNull(TEXT("AIController"), AIController);
	TestNotNull(TEXT("Attackers CombatStatsComponent"), Attacker->GetCombatStatsComponent());

	UBrainComponent* BrainComponent = NewObject<UBrainComponent>(GetTransientPackage(), UBrainComponent::StaticClass());
	TestNotNull(TEXT("BrainComponent"), BrainComponent);
	
	AIController->BrainComponent = BrainComponent;
	TestEqual(TEXT("BrainComponent running"), BrainComponent->IsRunning(), true);
	
	Victim->Controller = AIController;
	
	AWeaponBase* Weapon = World->SpawnActor<AWeaponBase>(AWeaponBase::StaticClass());
	TestNotNull(TEXT("Attackers weapon"), Weapon);
	Attacker->GetCombatStatsComponent()->Weapon = Weapon;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickUpMaxHealthIncrease, "Custom.Stats.StatIncrease.PickUp.MaxHealth", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPickUpMaxHealthIncrease::RunTest(const FString& Parameters)
{
	// GIVEN
	ACollectable* Collectable = NewObject<ACollectable>(GetTransientPackage(), ACollectable::StaticClass());
	ACharacterBase* Character = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Collectable"), Collectable);
	TestNotNull(TEXT("Character"), Character);
	TestNotNull(TEXT("Collectables CollectablesStatsComponent"), Collectable->GetCollectableStatsComponent());
	TestNotNull(TEXT("Characters HealthComponent"), Character->GetHealthComponent());

	Character->GetHealthComponent()->MaxHealth = 100;

	Collectable->GetCollectableStatsComponent()->CollectableType = ECollectableType::MaxHealth;
	Collectable->GetCollectableStatsComponent()->MaxHealthIncrease = 25;

	// WHEN
	UCombatService::ApplyStatIncrease(Collectable, Character);

	// THEN
	int32 ExpectedHealth = 100 + 25; // MaxHealth + CollectableStatIncrease
	return TestEqual(TEXT("CurrentHealth"), Character->GetHealthComponent()->MaxHealth, ExpectedHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickUpCurrentHealthIncrease, "Custom.Stats.StatIncrease.PickUp.CurrentHealth", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPickUpCurrentHealthIncrease::RunTest(const FString& Parameters)
{
	// GIVEN
	ACollectable* Collectable = NewObject<ACollectable>(GetTransientPackage(), ACollectable::StaticClass());
	ACharacterBase* Character = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Collectable"), Collectable);
	TestNotNull(TEXT("Character"), Character);
	TestNotNull(TEXT("Collectables CollectablesStatsComponent"), Collectable->GetCollectableStatsComponent());
	TestNotNull(TEXT("Characters HealthComponent"), Character->GetHealthComponent());

	Character->GetHealthComponent()->CurrentHealth = 50;

	Collectable->GetCollectableStatsComponent()->CollectableType = ECollectableType::Heal;
	Collectable->GetCollectableStatsComponent()->HealAmount = 25;

	// WHEN
	UCombatService::ApplyStatIncrease(Collectable, Character);

	// THEN
	int32 ExpectedHealth = 50 + 25; // CurrentHealth + CollectableStatIncrease
	return TestEqual(TEXT("CurrentHealth"), Character->GetHealthComponent()->CurrentHealth, ExpectedHealth);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPickUpCurrentDamageIncrease, "Custom.Stats.StatIncrease.PickUp.Damage", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FPickUpCurrentDamageIncrease::RunTest(const FString& Parameters)
{
	// GIVEN
	ACollectable* Collectable = NewObject<ACollectable>(GetTransientPackage(), ACollectable::StaticClass());
	ACharacterBase* Character = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());
	TestNotNull(TEXT("Collectable"), Collectable);
	TestNotNull(TEXT("Character"), Character);
	TestNotNull(TEXT("Collectables CollectablesStatsComponent"), Collectable->GetCollectableStatsComponent());
	TestNotNull(TEXT("Characters CombatStatsComponent"), Character->GetCombatStatsComponent());

	AWeaponBase* Weapon = NewObject<AWeaponBase>(GetTransientPackage(), AWeaponBase::StaticClass());
	TestNotNull(TEXT("Weapon"), Weapon);
	TestNotNull(TEXT("Weapons WeaponStatsComponent"), Weapon->GetWeaponStatsComponent());
	Character->GetCombatStatsComponent()->Weapon = Weapon;

	Weapon->GetWeaponStatsComponent()->Damage = 34;

	Collectable->GetCollectableStatsComponent()->CollectableType = ECollectableType::Damage;
	Collectable->GetCollectableStatsComponent()->DamageMultiplier = 1.1f;

	// WHEN
	UCombatService::ApplyStatIncrease(Collectable, Character);

	// THEN
	int32 ExpectedDamageStat = 34 * 1.1f; // Weapons DamageStat * Multiplier
	return TestEqual(TEXT("Weapons Damage Stat"), Weapon->GetWeaponStatsComponent()->Damage, ExpectedDamageStat);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFloatUpDown, "Custom.Movement.FloatingUpDown", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FFloatUpDown::RunTest(const FString& Parameters)
{
	// GIVEN
	ACollectable* Collectable = NewObject<ACollectable>(GetTransientPackage(), ACollectable::StaticClass());
	TestNotNull(TEXT("Collectable"), Collectable);
	TestNotNull(TEXT("Collectables CollectableStatsComponent"), Collectable->GetCollectableStatsComponent());

	float InitialHeight = Collectable->GetActorLocation().Z;
	float Distance = 50.f;
	float Speed = 1.f;
	
	float DeltaTime = 0.01f; // Equals 100 FPS
	// UMovementService:FloatUpDown setzt die Position entsprechend einer Sinuskurve, die sich aus dem Sinus (in Radians) der GameTimeSinceCreation ergibt.
	// Für das Erreichen der DownDistance braucht es dementsprechend nicht den gesamten Kreis, sondern nur 3/4
	int32 NumberOfFrames = 475;
	float ActorGameTimeSinceCreation = 0.f;

	bool bUpDistanceReached = false;
	bool bDownDistanceReached = false;

	// WHEN
	for (int32 i = 0; i <= NumberOfFrames; i++)
	{
		UMovementService::FloatUpDown(Collectable, InitialHeight, Distance, Speed, ActorGameTimeSinceCreation);
		ActorGameTimeSinceCreation += DeltaTime;
		
		// THEN
		if (Collectable->GetActorLocation().Z >= InitialHeight + Distance - 1)
		{
			bUpDistanceReached = true;
		}
		else if (Collectable->GetActorLocation().Z <= InitialHeight - Distance + 1)
		{
			bDownDistanceReached = true;
		}
	}
	
	return TestTrue(TEXT("Up and down distance reached in time"), bUpDistanceReached && bDownDistanceReached);
}

// TODO MoveByInputAxis is not testable because the Tick function of the CharacterMovementComponent handles the movement.
// TODO I can call the Tick function, but the function returnes before applying the movement because external variables are probably not valid.
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveForward, "Custom.NotWorking.Movement.MoveForward", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMoveForward::RunTest(const FString& Parameters)
{
	return true;
	
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	TestNotNull(TEXT("World"), World);

	APlayerCharacter* PlayerCharacter = World->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass());
	TestNotNull(TEXT("PlayerCharacter"), PlayerCharacter);

	APlayerController* PlayerController = World->SpawnActor<APlayerController>(APlayerController::StaticClass());
	TestNotNull(TEXT("Controller"), PlayerController);

	PlayerCharacter->Controller = PlayerController;
	
	PlayerCharacter->Tick(1.f);
	PlayerCharacter->GetCharacterMovement()->TickComponent(1.f, LEVELTICK_All, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacter->GetActorLocation().ToString());

	PlayerCharacter->Tick(1.f);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacter->GetActorLocation().ToString());

	PlayerCharacter->Tick(1.f);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerCharacter->GetActorLocation().ToString());

	TSoftObjectPtr<UAnimMontage> TestMontage;
	// TestMontage.LoadSynchronous()
	
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FWaitFOrAnimation, UAnimInstance*, AnimInstance);

bool FWaitFOrAnimation::Update()
{
	if (AnimInstance->Montage_IsPlaying(nullptr))
	{
		return true;
	}
	else
	{
		return false;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsInLockingAnimation, "Custom.NotWorking.Animation.IsInLockingAnimation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FIsInLockingAnimation::RunTest(const FString& Parameters)
{
	return true;
	
	// GIVEN
	ACharacterBase* Character = NewObject<ACharacterBase>(GetTransientPackage(), ACharacterBase::StaticClass());	
	UAnimInstance* AnimInstance = NewObject<UAnimInstance>(Character->GetMesh(), UAnimInstance::StaticClass());
	TestNotNull(TEXT("Character"), Character);
	TestNotNull(TEXT("AnimInstance"), AnimInstance);

	USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/StarterContent/Characters/Mannequins/Meshes/SKM_Quinn"));
	TestNotNull(TEXT("SkeletalMesh"), SkeletalMesh);

	UAnimMontage* AnimMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Game/Assets/Animations/AM_Primary_Attack_A"));
	TestNotNull(TEXT("AnimMontage"), AnimMontage);
	
	Character->GetMesh()->AnimScriptInstance = AnimInstance;
	TestNotNull(TEXT("Characters AnimInstance"), Character->GetMesh()->GetAnimInstance());

	Character->GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
	// TestEqual(TEXT("IsPlaying"), Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AnimMontage), true);
	ADD_LATENT_AUTOMATION_COMMAND(FWaitFOrAnimation(Character->GetMesh()->GetAnimInstance()));
	// I can't get the animation to play
	
	return true;

	// for (auto& AssetData : AssetDataArray)
	// {
	// 	if (AssetData.GetClass() == UAnimMontage::StaticClass())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Jup"));
	// 	}
	// }

	// WHEN
	

	return true;
}
