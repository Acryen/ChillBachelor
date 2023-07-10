// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class BACHELOR_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

private:
	/*
	 * Components
	 */

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	//
	// Components - DataLayer

	UPROPERTY(VisibleAnywhere, Category = WeaponStats)
	class UWeaponStatsComponent* WeaponStatsComponent;

	/*
	 * Callback functions
	 */

	UFUNCTION()
	void OnStaticMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//
	// Callback functions - No Overlap duplicates

	UPROPERTY()
	TArray<AActor*> HitBlacklistedActorsArray;

public:
	void EmptyHitBlacklistArray();


public:
	/*
	 * Getters
	 */

	FORCEINLINE UWeaponStatsComponent* GetWeaponStatsComponent() { return WeaponStatsComponent; }
};
