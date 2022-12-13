// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestExamplePr_gitGameMode.h"
#include "TestExamplePr_gitCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestExamplePr_gitGameMode::ATestExamplePr_gitGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
