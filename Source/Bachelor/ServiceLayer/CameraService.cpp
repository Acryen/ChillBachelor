#include "CameraService.h"

#include "InputActionValue.h"

void UCameraService::MoveCameraByInputAxis(APawn* UserPawn, const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (UserPawn && UserPawn->Controller)
	{
		// add yaw and pitch input to controller
		UserPawn->AddControllerYawInput(LookAxisVector.X);
		UserPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}
