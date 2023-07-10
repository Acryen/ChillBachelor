// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

struct FAIStimulus;

UCLASS()
class BACHELOR_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;

private:
	/*
	 * Components
	 */

	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* AIPerceptionComponent;

	/*
	 * Callback functions
	 */

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus);

	/*
	 * Miscellaneous
	 */

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;
	
};
