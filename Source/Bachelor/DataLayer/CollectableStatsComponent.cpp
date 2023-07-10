#include "CollectableStatsComponent.h"



UCollectableStatsComponent::UCollectableStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCollectableStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialHeight = GetOwner()->GetActorLocation().Z;
}

