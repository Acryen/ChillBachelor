﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO Tick ist gerade im Constructor disabled
}
