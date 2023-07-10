#pragma once

#include "CoreMinimal.h"

struct FInputActionValue;

class UCameraService
{
public:
	static void MoveCameraByInputAxis(APawn* UserPawn, const FInputActionValue& Value);
	
};
