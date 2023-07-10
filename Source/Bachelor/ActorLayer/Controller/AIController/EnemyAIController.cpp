// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!BehaviorTree) return;
	RunBehaviorTree(BehaviorTree);
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ThisClass::OnTargetPerceptionUpdated);
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* SourceActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerCharacter"), SourceActor);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(TEXT("PlayerCharacter"));
	}
}
