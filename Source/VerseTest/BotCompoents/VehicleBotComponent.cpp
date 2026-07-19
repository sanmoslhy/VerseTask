// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCompoents/VehicleBotComponent.h"
#include"VerseTest\VerseTestPawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include"VerseTest\BotWaypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Sort.h"
#include "HAL/IConsoleManager.h"
static TAutoConsoleVariable<int32> CVarBotDiag(
	TEXT("yourbot.Diag"),
	0,
	TEXT("Enable Bot Diagnostics")
);
UVehicleBotComponent::UVehicleBotComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UVehicleBotComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwner()->HasAuthority())
	{
		SetComponentTickEnabled(false);
		return;
	}

	OwnerVehicle = Cast<AVerseTestPawn>(GetOwner());

	if (!OwnerVehicle)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is not VerseTestPawn"));
		SetComponentTickEnabled(false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("VehicleBotComponent Initialized"));
	NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	if (!NavSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Navigation System Not Found"));
		return;
	}
	
	TArray<AActor*> FoundWaypoints;

	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		ABotWaypoint::StaticClass(),
		FoundWaypoints);

	PatrolPoints.Empty();

	for (AActor* Actor : FoundWaypoints)
	{
		if (ABotWaypoint* Waypoint = Cast<ABotWaypoint>(Actor))
		{
			PatrolPoints.Add(Waypoint);
		}
	}

	PatrolPoints.Sort([](const ABotWaypoint& A, const ABotWaypoint& B)
		{
			return A.WaypointIndex < B.WaypointIndex;
		});
	for (ABotWaypoint* Waypoint : PatrolPoints)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Waypoint %d"),
			Waypoint->WaypointIndex);
	}

	if (PatrolPoints.Num() > 0)
	{
		CurrentWaypointIndex = 0;

		TargetLocation =
			PatrolPoints[CurrentWaypointIndex]->GetActorLocation();

		BulidPath();
	}
}


// Called every frame
void UVehicleBotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!GetOwner()->HasAuthority())
	{
		return;
	}
	
	if (!OwnerVehicle)
	{
		return;
	}
	UpdateRecovery(DeltaTime);
	if (bIsRecovering)
	{
		return;
	}
	UpdatePathFollowing(DeltaTime);
	UpdateWaypoint();
	UpdateSteering(DeltaTime);
	UpdateThrottle(DeltaTime);
	DiagnosticTimer += DeltaTime;
	
	if (CVarBotDiag.GetValueOnGameThread() != 0 && DiagnosticTimer >= DiagnosticInterval)
	{
		DiagnosticTimer = 0.f;
		PrintDiagnostics();
	}
	DrawDebugSphere(
		GetWorld(),
		TargetLocation,
		100.f,
		16,
		FColor::Green,
		false,
		0.f);
}



void UVehicleBotComponent::BulidPath()
{
	if (!NavSystem || !OwnerVehicle)
	{
		return;
	}

	CurrentPath = NavSystem->FindPathToLocationSynchronously(
		GetWorld(),
		OwnerVehicle->GetActorLocation(),
		TargetLocation);

	if (!CurrentPath || !CurrentPath->IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Path"));
		CurrentState = EBotState::Idle;
		NoProgressReason = EBotNoProgressReason::NoPath;
		return;
	}
	CurrentPathPointIndex = 1;
	for (const FVector& Point : CurrentPath->PathPoints)
	{
		DrawDebugSphere(
			GetWorld(),
			Point,
			40.f,
			12,
			FColor::Red,
			false,
			5.f);
	}
}

bool UVehicleBotComponent::GetNextPoint(FVector& OutPoint) const
{
	if (!CurrentPath)
	{
		return false;
	}

	if (!CurrentPath->PathPoints.IsValidIndex(CurrentPathPointIndex))
	{
		return false;
	}

	OutPoint = CurrentPath->PathPoints[CurrentPathPointIndex];

	return true;
}

void UVehicleBotComponent::UpdatePathFollowing(float DeltaTime)
{
	

	if (GetNextPoint(NextPoint))
	{
		DrawDebugSphere(
			GetWorld(),
			NextPoint,
			50.f,
			12,
			FColor::Green,
			false,
			0.f);
		const float PathPointDistance =
			FVector::Dist(
				OwnerVehicle->GetActorLocation(),
				NextPoint);

		if (PathPointDistance < PathPointAcceptanceRadius)
		{
			CurrentPathPointIndex++;

			CurrentPathPointIndex = FMath::Clamp(
				CurrentPathPointIndex,
				1,
				CurrentPath->PathPoints.Num() - 1);
		}
		
		
	}
}

void UVehicleBotComponent::UpdateWaypoint()
{
	const float Distance =
		FVector::Dist(
			OwnerVehicle->GetActorLocation(),
			TargetLocation);

	if (PatrolPoints.Num() > 0 && Distance < WaypointAcceptanceRadius)
	{
		CurrentWaypointIndex =
			(CurrentWaypointIndex + 1) % PatrolPoints.Num();

		TargetLocation =
			PatrolPoints[CurrentWaypointIndex]->GetActorLocation();

		BulidPath();
	}
}

void UVehicleBotComponent::UpdateSteering(float DeltaTime)
{
	CurrentTargetPoint = NextPoint;
	if (!GetNextPoint(CurrentTargetPoint))
	{
		return;
	}
	const FVector Direction =
		(NextPoint - OwnerVehicle->GetActorLocation()).GetSafeNormal();

	const FVector Local =
		OwnerVehicle->GetActorTransform().InverseTransformVectorNoScale(Direction);

	TargetSteering =
		FMath::Clamp(Local.Y, -1.f, 1.f);

	CurrentSteering =
		FMath::FInterpTo(
			CurrentSteering,
			TargetSteering,
			DeltaTime,
			SteeringInterpSpeed);

	
	OwnerVehicle->DoSteering(CurrentSteering);
}

void UVehicleBotComponent::UpdateThrottle(float DeltaTime)
{
	const float SteeringAmount = FMath::Abs(CurrentSteering);
	CurrentThrottleInput = CurrentThrottle;
	TargetThrottle =FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f),FVector2D(MaxThrottle, MinThrottle),SteeringAmount);

	CurrentThrottle =
		FMath::FInterpTo(
			CurrentThrottle,
			TargetThrottle,
			DeltaTime,
			ThrottleInterpSpeed);

	OwnerVehicle->DoThrottle(CurrentThrottle);
	CurrentState = EBotState::Driving;
	NoProgressReason = EBotNoProgressReason::None;
}

void UVehicleBotComponent::PrintDiagnostics()
{
	if (!OwnerVehicle)
	{
		return;
	}

	const float Speed =
		OwnerVehicle->GetVehicleMovementComponent()->GetForwardSpeed();

	UE_LOG(
		LogTemp,
		Display,
		TEXT("[BOT] Speed=%.1f cm/s | Waypoint=%d | State=%s | Reason=%s"),
		Speed,
		CurrentWaypointIndex,
		*UEnum::GetValueAsString(CurrentState),
		*UEnum::GetValueAsString(NoProgressReason));
}

void UVehicleBotComponent::UpdateRecovery(float DeltaTime)
{
	if (bIsRecovering)
	{
		RecoveryTimer += DeltaTime;
		CurrentState = EBotState::Recovering;
		NoProgressReason = EBotNoProgressReason::Recovering;
		UE_LOG(LogTemp, Warning, TEXT("Recovering..."));
		auto* Movement = OwnerVehicle->GetVehicleMovementComponent();

		Movement->SetTargetGear(-1, true);
		OwnerVehicle->DoBrake(0.f);
		OwnerVehicle->DoThrottle(-1.f);
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Throttle = %f"),
			OwnerVehicle->GetVehicleMovementComponent()->GetThrottleInput());
		OwnerVehicle->DoSteering(1.0f);

		if (RecoveryTimer >= RecoveryDuration)
		{
			StopRecovery();
		}

		return;
	}

	if (IsVehicleStuck())
	{
		CurrentState = EBotState::Stuck;
		NoProgressReason = EBotNoProgressReason::Blocked;
		StuckTimer += DeltaTime;

		if (StuckTimer >= StuckTimeThreshold)
		{
			StartRecovery();
		}
	}
	else
	{
		StuckTimer = 0.f;
	}
}

bool UVehicleBotComponent::IsVehicleStuck() const
{
	if (!OwnerVehicle)
	{
		return false;
	}

	const UChaosVehicleMovementComponent* Movement =OwnerVehicle->GetVehicleMovementComponent();

	if (!Movement)
	{
		return false;
	}
	
	const float Speed = Movement->GetForwardSpeed();
	if (CurrentThrottle < 0.2f)
	{
		return false;
	}
	return FMath::Abs(Speed) < MinMovementSpeed;
}

void UVehicleBotComponent::StartRecovery()
{
	bIsRecovering = true;
	RecoveryTimer = 0.f;
	auto* Movement = OwnerVehicle->GetVehicleMovementComponent();

	Movement->SetUseAutomaticGears(false);
	Movement->SetTargetGear(-1, true);
	OwnerVehicle->GetMesh()->AddImpulse(
		-OwnerVehicle->GetActorForwardVector() * RecoveryImpulse,
		NAME_None,
		true);
	UE_LOG(LogTemp, Warning, TEXT("Recovery Started"));
}

void UVehicleBotComponent::StopRecovery()
{

	bIsRecovering = false;

	StuckTimer = 0.f;
	RecoveryTimer = 0.f;
	auto* Movement = OwnerVehicle->GetVehicleMovementComponent();

	Movement->SetUseAutomaticGears(true);
	BulidPath();

	UE_LOG(LogTemp, Warning, TEXT("Recovery Finished"));
}


