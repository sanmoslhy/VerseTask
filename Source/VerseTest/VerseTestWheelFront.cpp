// Copyright Epic Games, Inc. All Rights Reserved.

#include "VerseTestWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UVerseTestWheelFront::UVerseTestWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}