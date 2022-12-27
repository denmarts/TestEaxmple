#pragma once

#include "TestExamplePr_git/Tests/Utils/InputRecordingUtils.h"
#include "AutomationBlueprintFunctionLibrary.h"

//В данном классе содержатся различные helper-функции, описание которых вы найдете в их реализациях (в .cpp файле)
class HelperFunctions
{
	public:
		struct multiparam_fps_ram
		{
			float FPS_or_RAM;
			FVector PlayerPosition;
			FRotator CameraRotation;
		};
		static UWorld* GetTestWorld();
		static int32 GetActionBindingsIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent);
		static int32 GetAxisBindingsIndexByName(UInputComponent* InputComp, const FString& AxisName);
		static int32 CountingObjectsOnTheMap(const UWorld* World, const AActor* Actor);
		static int32 CountingObjectsOnTheMap(const UWorld* World, UClass* ClassOfTheActor);
		static FString GetTestDataDir();
		static float GetFPS(const UWorld* World);
		static float GetAvarageFPS(TArray<float>* FPS);
		static multiparam_fps_ram GetFPSwithCurrentPosition(const UWorld* World, const ACharacter* Character);
		static void GetRAM(TArray<float>* RAM);
		static float GetAvarageRAM(TArray<float>* RAM);
		static multiparam_fps_ram GetRAMwithCurrentPosition(const ACharacter* Character);
		static bool WriteStringArrayIntoFile(const FString InFileName, TArray<FString> ParametersToWrite, bool Rewrite);
		static bool WriteStringIntoFile(const FString InFileName, FString StringToWrite);
};

//Класс позволяющий выполнять определенную функцию определенное количество времени.
class FTestExampleUntilCommand : public IAutomationLatentCommand
{
	public:
		FTestExampleUntilCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout = 5.0f)
			: Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout) {}

		virtual bool Update() override;

	private:
		TFunction<void()> Callback;
		TFunction<void()> TimeoutCallback;
		float Timeout;
};

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
	FSimulateMovementLatentCommand(const UWorld* InWorld, const UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindingsData)
		: World(InWorld), InputComponent(InInputComponent), BindingsData(InBindingsData) {}
    
	virtual bool Update() override;
    
private:
	const UWorld* World;
	const UInputComponent* InputComponent;
	const TArray<FBindingsData> BindingsData;
	int32 Index{0};
	float WorldStartTime{0.0f};
};

class FTakeScreenshotAutomationCommand : public IAutomationLatentCommand
{
	public:
		FTakeScreenshotAutomationCommand(const FString& InScreenshotName);
		virtual ~FTakeScreenshotAutomationCommand() override;
		virtual bool Update() override;
		
	private:
		const FString ScreenshotName;
		bool ScreenshotRequested = false;
		bool CommandCompleted = false;
	    void OnScreenshotTakenAndCompared() {CommandCompleted = true;}
};
