// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"

#include "Bachelor/ActorLayer/Weapon/WeaponBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/CombatStatsComponent.h"
#include "Bachelor/DataLayer/HealthComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AnimationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("AnimationComponent"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CombatStatsComponent = CreateDefaultSubobject<UCombatStatsComponent>(TEXT("CombatStatsComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	ConstructorHelpers::FClassFinder<AWeaponBase> DefaultWeaponBlueprint(TEXT("/Game/Blueprints/Actor/BP_WeaponBase"));
	if (DefaultWeaponBlueprint.Class)
	{
		CombatStatsComponent->WeaponBlueprint = DefaultWeaponBlueprint.Class;
	}
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!CombatStatsComponent->WeaponBlueprint) return;
	CombatStatsComponent->Weapon = GetWorld()->SpawnActor<AWeaponBase>(CombatStatsComponent->WeaponBlueprint);
	UWeaponStatsComponent* WeaponStatsComponent = CombatStatsComponent->Weapon->GetWeaponStatsComponent();
	if (!CombatStatsComponent->Weapon || !WeaponStatsComponent) return;
	
	CombatStatsComponent->Weapon->SetOwner(this);
	CombatStatsComponent->Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r"));
	CombatStatsComponent->Weapon->SetActorRelativeLocation(WeaponStatsComponent->WeaponLocation);
	CombatStatsComponent->Weapon->SetActorRelativeRotation(WeaponStatsComponent->WeaponRotation);
	CombatStatsComponent->Weapon->SetActorRelativeScale3D(WeaponStatsComponent->WeaponScale);
}
