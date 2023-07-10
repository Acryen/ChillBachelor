#include "MovementService.h"

#include "AnimationService.h"
#include "InputActionValue.h"


void UMovementService::MoveByInputAxis(APawn* UserPawn, const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (!UAnimationService::IsInMovementBlockingAnimation(UserPawn) && UserPawn && UserPawn->Controller)
	{
		// find out which way is forward
		const FRotator Rotation = UserPawn->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		UserPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		UserPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void UMovementService::FloatUpDown(AActor* Actor, float InitialHeight, float Distance, float Speed, float ActorGameTimeSinceCreation)
{
	if (!Actor) return;	
	
	float TargetZ = InitialHeight + Distance * FMath::Sin(ActorGameTimeSinceCreation * Speed);

	FVector CurrentLocation = Actor->GetActorLocation();
	FVector NewLocation = FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ);
	Actor->SetActorLocation(NewLocation);
}