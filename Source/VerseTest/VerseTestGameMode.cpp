// Copyright Epic Games, Inc. All Rights Reserved.

#include "VerseTestGameMode.h"
#include "VerseTestPlayerController.h"

AVerseTestGameMode::AVerseTestGameMode()
{
	PlayerControllerClass = AVerseTestPlayerController::StaticClass();
}
