#include "Collectable.h"

#include "Bachelor/ActorLayer/Character/CharacterBase.h"
#include "Bachelor/DataLayer/CollectableStatsComponent.h"
#include "Bachelor/ServiceLayer/CombatService.h"
#include "Bachelor/ServiceLayer/MovementService.h"


ACollectable::ACollectable()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	CollectableStatsComponent = CreateDefaultSubobject<UCollectableStatsComponent>(TEXT("ColleactableStatsComp"));
}

void ACollectable::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
}

void ACollectable::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		UCombatService::ApplyStatIncrease(this, Character);
		Destroy();
	}
}

void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UMovementService::FloatUpDown(
		this,
		CollectableStatsComponent->InitialHeight,
		CollectableStatsComponent->FloatingDistance,
		CollectableStatsComponent->FloatingSpeed,
		GetGameTimeSinceCreation());
}

