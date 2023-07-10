// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable.generated.h"

UCLASS()
class BACHELOR_API ACollectable : public AActor
{
	GENERATED_BODY()

public:
	ACollectable();
	
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	/*
	 * Components
	 */

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	//
	// Comonents - DataLayer

	UPROPERTY(VisibleAnywhere)
	class UCollectableStatsComponent* CollectableStatsComponent;

	/*
	 * Callback functions
	 */

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

public:
	/*
	 * Getters
	 */

	FORCEINLINE UCollectableStatsComponent* GetCollectableStatsComponent() { return CollectableStatsComponent; }
};
