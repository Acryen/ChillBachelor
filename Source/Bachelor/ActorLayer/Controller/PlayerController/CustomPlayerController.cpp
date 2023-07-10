

#include "CustomPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Bachelor/ServiceLayer/AnimationService.h"
#include "Bachelor/ServiceLayer/CameraService.h"
#include "Bachelor/ServiceLayer/MovementService.h"


ACustomPlayerController::ACustomPlayerController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("BeginPlay")));
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Started, this, &ThisClass::LightAttack);
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);

		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ThisClass::StartBlock);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ThisClass::EndBlock);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SetupInput")));
	}
}


void ACustomPlayerController::Move(const FInputActionValue& Value)
{
	UMovementService::MoveByInputAxis(GetPawn(), Value);
}

void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	UCameraService::MoveCameraByInputAxis(GetPawn(), Value);
}

void ACustomPlayerController::LightAttack(const FInputActionValue& Value)
{
	UAnimationService::PlayLightAttackMontage(GetPawn());
}

void ACustomPlayerController::HeavyAttack(const FInputActionValue& Value)
{
	// To be done
}

void ACustomPlayerController::StartBlock(const FInputActionValue& Value)
{
	UAnimationService::PlayBlockMontage(GetPawn());
}

void ACustomPlayerController::EndBlock(const FInputActionValue& Value)
{
	UAnimationService::StopBlockMontage(GetPawn());
}

