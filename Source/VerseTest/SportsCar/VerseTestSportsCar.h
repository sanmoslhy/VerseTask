// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VerseTestPawn.h"
#include "VerseTestSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class AVerseTestSportsCar : public AVerseTestPawn
{
	GENERATED_BODY()
	
public:

	AVerseTestSportsCar();
};
