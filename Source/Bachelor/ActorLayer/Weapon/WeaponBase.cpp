// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Bachelor/ActorLayer/Character/CharacterBase.h"
#include "Bachelor/DataLayer/AnimationComponent.h"
#include "Bachelor/DataLayer/WeaponStatsComponent.h"
#include "Bachelor/ServiceLayer/CombatService.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootSceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetGenerateOverlapEvents(true);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	WeaponStatsComponent = CreateDefaultSubobject<UWeaponStatsComponent>(TEXT("WeaponStatsComponent"));
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnStaticMeshBeginOverlap);
}

void AWeaponBase::OnStaticMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || HitBlacklistedActorsArray.Contains(OtherActor)) return;
	
	ACharacterBase* OwnerCharacter = Cast<ACharacterBase>(GetOwner());
	if (!OwnerCharacter) return;

	UAnimationComponent* OwnerAnimationComponent = OwnerCharacter->GetAnimationComponent();
	if (!OwnerAnimationComponent) return;
	
	ACharacterBase* OtherCharacter = Cast<ACharacterBase>(OtherActor);
	if (!OtherCharacter) return;

	if (OwnerAnimationComponent->CombatState == ECombatState::Attacking)
	{
		UCombatService::ApplyDamage(OwnerCharacter, OtherCharacter);
		HitBlacklistedActorsArray.Add(OtherActor);
	}
}

void AWeaponBase::EmptyHitBlacklistArray()
{
	HitBlacklistedActorsArray.Empty();
}

