// Copyright Epic Games, Inc. All Rights Reserved.

#include "VerseTestWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UVerseTestWheelRear::UVerseTestWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}