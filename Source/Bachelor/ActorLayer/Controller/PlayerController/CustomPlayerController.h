// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class BACHELOR_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACustomPlayerController();
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;

	/*
	 * Input
	 */
	
	
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

	//
	// Input functions
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void LightAttack(const FInputActionValue& Value);
	void HeavyAttack(const FInputActionValue& Value);
	void StartBlock(const FInputActionValue& Value);
	void EndBlock(const FInputActionValue& Value);
};
