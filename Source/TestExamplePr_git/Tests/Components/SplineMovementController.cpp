// Fill out your copyright notice in the Description page of Project Settings.


#include "TestExamplePr_git/Tests/Components/SplineMovementController.h"
#include "Components/SplineComponent.h"

ASplineMovementController::ASplineMovementController()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	bCanMoveActor = false;
	PrimaryActorTick.bCanEverTick = true;
	ParametersAdded = false;
}

void ASplineMovementController::BeginPlay()
{
	Super::BeginPlay();
	if (ActorToMoveClass)
	{
		ActorToMove = World->SpawnActor<AActor>(ActorToMoveClass, Spline->GetComponentTransform());
		APlayerController* PC = World->GetFirstPlayerController();
		PC->SetViewTarget(ActorToMove);
		if (ActorToMove)
		{
			StartTime = World->GetTimeSeconds();
			bCanMoveActor = true;
		}
	}
}

void ASplineMovementController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActorToMove && bCanMoveActor)
	{
		float CurrentSplineTime = (World->GetTimeSeconds() - StartTime) / TotalPathTimeController;
		const float Distance = Spline->GetSplineLength() * CurrentSplineTime;

		const FVector Position = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		ActorToMove->SetActorLocation(Position);
		
		const FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		const FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
		ActorToMove->SetActorRotation(Rotator);
		
		if (CurrentSplineTime >= 1.0f)
		{
			if (Loop)
			{
				bCanMoveActor = true;
				StartTime = World->GetTimeSeconds();
				CurrentSplineTime = (World->GetTimeSeconds() - StartTime) / TotalPathTimeController;
			}
		}
	}
}

void ASplineMovementController::AddParameters(UWorld* InWorld, TSubclassOf<class AActor> InActorToMoveClass, bool InLoop, float InTotalPathTimeController)
{
	if(!ParametersAdded)
	{
		if(InWorld && InActorToMoveClass)
		{
			World = InWorld;
			ActorToMoveClass = InActorToMoveClass;
			InTotalPathTimeController <= 0.0f ? TotalPathTimeController = 10.0f : TotalPathTimeController = InTotalPathTimeController;
			Spline->Duration = TotalPathTimeController;
			Spline->bDrawDebug = true;
			ParametersAdded = true;
		}
	}
}

void ASplineMovementController::MakeYourOwnSpline(TArray<UE::Math::TVector<double>>& InPoints) const
{
	if(InPoints.Num() > 0)
	{
		for(int32 i = 0; i < 4; i++)
		{
			Spline->SplineCurves.Position.Points.Pop();
		}
		for(int32 i = 0; i < InPoints.Num(); i++)
		{
			Spline->SplineCurves.Position.AddPoint(i, InPoints[i]);
		}
	}
}



