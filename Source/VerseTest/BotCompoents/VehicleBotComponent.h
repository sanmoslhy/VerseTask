// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleBotComponent.generated.h"

class AVerseTestPawn;
class UNavigationSystemV1;
class UNavigationPath;
class ABotWaypoint;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VERSETEST_API UVehicleBotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleBotComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void BulidPath();
	bool GetNextPoint(FVector& OutPoint) const;
	void UpdatePathFollowing(float DeltaTime);
	void UpdateWaypoint();
	void UpdateSteering(float DeltaTime);
	void UpdateThrottle(float DeltaTime);

	//Recovery Functions
	void UpdateRecovery(float DeltaTime);

	bool IsVehicleStuck() const;

	void StartRecovery();

	void StopRecovery();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY()
	TObjectPtr<AVerseTestPawn> OwnerVehicle;
	UPROPERTY()
	TObjectPtr<UNavigationPath> CurrentPath;

	UNavigationSystemV1* NavSystem = nullptr;
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector TargetLocation = FVector(5000.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<ABotWaypoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	int32 CurrentWaypointIndex = 0;
	UPROPERTY(VisibleAnywhere, Category = "AI")
	int32 CurrentPathPointIndex = 1;
	UPROPERTY(VisibleAnywhere, Category = "AI|Driving")
	float CurrentSteering = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "AI|Driving")
	float TargetSteering = 0.f;

	UPROPERTY(EditAnywhere, Category = "AI|Driving")
	float SteeringInterpSpeed = 5.f;
	UPROPERTY(VisibleAnywhere, Category = "AI|Driving")
	float CurrentThrottle = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "AI|Driving")
	float TargetThrottle = 0.f;

	UPROPERTY(EditAnywhere, Category = "AI|Driving")
	float ThrottleInterpSpeed = 3.f;

	UPROPERTY(EditAnywhere, Category = "AI|Driving")
	float MaxThrottle = 1.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Driving")
	float MinThrottle = 0.3f;
	UPROPERTY(VisibleAnywhere, Category = "AI|Recovery")
	bool bIsRecovering = false;

	UPROPERTY(VisibleAnywhere, Category = "AI|Recovery")
	float StuckTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = "AI|Recovery")
	float StuckTimeThreshold = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI|Recovery")
	float MinMovementSpeed = 20.f;
	UPROPERTY(VisibleAnywhere, Category = "AI|Recovery")
	float RecoveryTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = "AI|Recovery")
	float RecoveryDuration = 1.5f;
	UPROPERTY(EditAnywhere, Category = "AI|Recovery")
	float RecoveryImpulse = 80000.f;
	FVector CurrentTargetPoint;
	float CurrentThrottleInput;
	FVector NextPoint;
		
};
