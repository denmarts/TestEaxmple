// Fill out your copyright notice in the Description page of Project Settings.


#include "TestExamplePr_git/Tests/AI/AIFollowTheCharacterTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UAIFollowTheCharacterTask::UAIFollowTheCharacterTask()
{
	NodeName = "FollowThePlayer";
}

EBTNodeResult::Type UAIFollowTheCharacterTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(!Controller || !Blackboard) return EBTNodeResult::Failed;

	const auto Pawn = Controller->GetPawn();
	if(!Pawn) return EBTNodeResult::Failed;
	
	const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(Controller->GetWorld(), 0);
	if(!PlayerCharacter) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, PlayerCharacter->GetActorLocation());
	return EBTNodeResult::Succeeded;
}
