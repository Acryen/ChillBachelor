// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class BACHELOR_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*
	 * Components
	 */
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	//
	// Components - Input
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LightAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* BlockAction;

	/*
	 * Input functions
	 */
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LightAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);
	void StartBlock(const FInputActionValue& Value);
	void EndBlock(const FInputActionValue& Value);
};
