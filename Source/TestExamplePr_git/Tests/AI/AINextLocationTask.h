// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AINextLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class TESTEXAMPLEPR_GIT_API UAINextLocationTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAINextLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector AimLocationKey;
	
};
