// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineMovementController.generated.h"

UCLASS()
class TESTEXAMPLEPR_GIT_API ASplineMovementController : public AActor
{
	GENERATED_BODY()

public:

	ASplineMovementController();
	void AddParameters(UWorld* InWorld, TSubclassOf<class AActor> InActorToMoveClass, bool InLoop, float TotalPathTimeController);
	void MakeYourOwnSpline(TArray<UE::Math::TVector<double>>& Points) const;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	USceneComponent *Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SplineController")
	class USplineComponent* Spline;
	
	UWorld* World;
	TSubclassOf<class AActor> ActorToMoveClass;
	AActor* ActorToMove;
	
protected:

	UPROPERTY(EditAnywhere, Category = "SplineController")
	float TotalPathTimeController;

	bool bCanMoveActor;
	bool Loop;
	bool ParametersAdded;
	float StartTime;
	
protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};
