// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunctionalTest.h"
#include "ApplyDamageTest.generated.h"

UCLASS()
class BACHELOREDITOR_API AApplyDamageTest : public AFunctionalTest
{
	GENERATED_BODY()

public:
	AApplyDamageTest();

private:
	virtual void StartTest() override;
};
