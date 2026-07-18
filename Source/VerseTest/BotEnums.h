#pragma once

#include "CoreMinimal.h"
#include "BotEnums.generated.h"

UENUM(BlueprintType)
enum class EBotState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Driving     UMETA(DisplayName = "Driving"),
    Recovering  UMETA(DisplayName = "Recovering"),
    Stuck       UMETA(DisplayName = "Stuck")
};
UENUM(BlueprintType)
enum class EBotNoProgressReason : uint8
{
    None,
    NoPath,
    Blocked,
    Recovering
};