// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIFollowTheCharacterTask.generated.h"

/**
 * 
 */
UCLASS()
class TESTEXAMPLEPR_GIT_API UAIFollowTheCharacterTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UAIFollowTheCharacterTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FBlackboardKeySelector AimLocationKey;
};
