#include "HelperFunctions.h"

#include "GameFramework/Character.h"
#include "HAL/PlatformMemoryHelpers.h"
#include "Kismet/GameplayStatics.h"
//#include "HAL/Thread.h"


UWorld* HelperFunctions::GetTestWorld() 
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if (((Context.WorldType == EWorldType::PIE) || (Context.WorldType == EWorldType::Game)) && (Context.World() != nullptr))
        {
            return Context.World();
        }
    }
    return nullptr;
}

//возвращеает бинд экшна по введеному имени, если такой бинд существует
int32 HelperFunctions::GetActionBindingsIndexByName(UInputComponent* InputComp, const FString& ActionName, EInputEvent InputEvent)
{
    if(!InputComp)
    {
        return INDEX_NONE;
    }
    for(int32 i = 0; i < InputComp->GetNumActionBindings(); ++i)
    {
        const FInputActionBinding Action = InputComp->GetActionBinding(i);
        if(Action.GetActionName().ToString().Equals(ActionName) && Action.KeyEvent == InputEvent)
        {
                return i;
        }
    }
    return INDEX_NONE;
}

    //возвращеает бинд оси по введеному имени, если такой бинд существует
int32 HelperFunctions::GetAxisBindingsIndexByName(UInputComponent* InputComp, const FString& AxisName)
{
    if(!InputComp)
    {
        return INDEX_NONE;
    }  
    for(int32 i = 0; i < InputComp->AxisBindings.Num(); ++i)
    {
        if(InputComp->AxisBindings[i].AxisName.ToString().Equals(AxisName))
        {
            return i;
        }
    }
    return 0; //Если данный бинд не будет найден, то вернем 0 (движение вперед)
}

//Данная функция считает количество компонентов типа Actor, которые в данный момент находятся на карте
int32 HelperFunctions::CountingObjectsOnTheMap(const UWorld* World, const AActor* Actor)
{
    if(!World || !Actor)
    {
        return -1;
    }
    TArray<AActor*> Items;
    UGameplayStatics::GetAllActorsOfClass(World, Actor->GetClass(), Items);
    return Items.Num();
}

    //Перегруженная функция для той, что выше. В отличии от того, что выше принимает непосредственно класс, а не актора
int32 HelperFunctions::CountingObjectsOnTheMap(const UWorld* World, UClass* ClassOfTheActor)
{
    if(!World || !ClassOfTheActor)
    {
        return -1;
    } 
    TArray<AActor*> Items;
    UGameplayStatics::GetAllActorsOfClass(World, ClassOfTheActor, Items);
    return Items.Num();
}

FString HelperFunctions::GetTestDataDir()
{
    return FPaths::GameSourceDir().Append("TestExamplePr_git/Tests/Data/");
}

float HelperFunctions::GetFPS(const UWorld* World)
{
    if(World)
    {
       return 1.0f / World->GetDeltaSeconds();
    }
    return -1.0f;
}

float HelperFunctions::GetAvarageFPS(TArray<float>* FPS)
{
    if(FPS->Num() > 0)
    {
        float avarageFPS = 0.0f;
        for(const auto currentFPS : *FPS)
        {
            avarageFPS += currentFPS;
        }
        return avarageFPS /= FPS->Num();
    }
    return -1.0f;
}

float HelperFunctions::GetAvarageFPS(TArray<float> FPS)
{
    if(FPS.Num() > 0)
    {
        float avarageFPS = 0.0f;
        for(const auto currentFPS : FPS)
        {
            avarageFPS += currentFPS;
        }
        return avarageFPS /= FPS.Num();
    }
    return -1.0f;
}

HelperFunctions::multiparam_fps_ram HelperFunctions::GetFPSwithCurrentPosition(const UWorld* World, const ACharacter* Character)
{
    if(World && Character)
    {
        return {1.0f / World->GetDeltaSeconds(), Character->GetActorLocation(), Character->GetViewRotation()};
    }
    return {-1.0, FVector{}, FRotator{}};
}

void HelperFunctions::GetRAM(TArray<float>* RAM)
{
    if(RAM)
    {
        RAM->Add(PlatformMemoryHelpers::GetFrameMemoryStats().UsedPhysical / 1.0e+6);
    }
}

float HelperFunctions::GetAvarageRAM(TArray<float>* RAM)
{
    if(RAM->Num() > 0)
    {
        float avarageRAM = 0.0f;
        for(const auto currentFPS : *RAM)
        {
            avarageRAM += currentFPS;
        }
        return avarageRAM /= RAM->Num();
    }
    return -1.0f;
}

HelperFunctions::multiparam_fps_ram HelperFunctions::GetRAMwithCurrentPosition(const ACharacter* Character)
{
    if(Character)
    {
        return {static_cast<float>(PlatformMemoryHelpers::GetFrameMemoryStats().UsedPhysical / 1.0e+6), Character->GetActorLocation(), Character->GetViewRotation()};
    }
    return {-1.0, FVector{}, FRotator{}};
}

bool HelperFunctions::WriteStringArrayIntoFile(FString InFileName, TArray<FString> ParametersToWrite, bool Rewrite)
{
    const FString Filename = InFileName;
    FString File = FPaths::ProjectSavedDir();
    File.Append(TEXT("/Profiling/CSV/")).Append(Filename).Append(".csv");
    
    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
    if (FileManager.FileExists(*File) && Rewrite)
    {
        UE_LOG(LogTemp, Warning, TEXT("Warning, rewrite paramether is true. The existing data will be terminated"));
        FileManager.DeleteFile(*File);
    }

    if(!FFileHelper::SaveStringArrayToFile(ParametersToWrite, *File))
    {
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
        return false;
    }
    return true;
}

bool HelperFunctions::WriteStringIntoFile(FString InFileName, FString ParameterToWrite)
{
    const FString Filename = InFileName;
    FString File = FPaths::ProjectSavedDir();
    File.Append(TEXT("/Profiling/CSV/")).Append(Filename).Append(".csv");
    
    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
    if (FileManager.FileExists(*File))
    {
        UE_LOG(LogTemp, Warning, TEXT("Warning, rewrite paramether is true. The existing data will be terminated"));
        FileManager.DeleteFile(*File);
    }

    if(!FFileHelper::SaveStringToFile(ParameterToWrite, *File))
    {
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
        return false;
    }
    return true;
}

/**
 * @FTestExampleUntilCommand
 */

bool FTestExampleUntilCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }
    Callback();
    return false;
}

/**
 * @FTestExampleUntilTrueCommand
 */

bool FTestExampleUntilTrueCommand::Update()
{
    if (CheckTimeout())
    {
        TimeoutCallback();
        return true;
    }
    Callback();
    return false;
}

/**
 * @FSimulateMovementLatentCommand 
 */

bool FSimulateMovementLatentCommand ::Update()
{
    if (!World || !InputComponent)
    {
        return true;
    }
    
    if (WorldStartTime == 0.0f)
    {
        WorldStartTime = World->TimeSeconds;
    }
    
    while (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
    {
        for (int32 i = 0; i < InputComponent->AxisBindings.Num(); ++i)
        {
            const float AxisValue = BindingsData[Index].AxisValues[i].Value;
            InputComponent->AxisBindings[i].AxisDelegate.Execute(AxisValue);
        }
        if (Index > 0)
        {
            for (int32 i = 0; i < InputComponent->GetNumActionBindings(); ++i)
            {
                const auto ActionValue = BindingsData[Index].ActionValues[i];
                const auto PrevActionValue = BindingsData[Index - 1].ActionValues[i];
                if (ActionValue.State && ActionValue.State != PrevActionValue.State)
                {
                    InputComponent->GetActionBinding(i).ActionDelegate.Execute(ActionValue.Key);
                }
            }
        }
        
        if (++Index >= BindingsData.Num())
        {
            return true;
        }
    }
    return false;
}

FTakeScreenshotAutomationCommand::FTakeScreenshotAutomationCommand(const FString& InScreenshotName): ScreenshotName(InScreenshotName)
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotAutomationCommand::OnScreenshotTakenAndCompared);
}

FTakeScreenshotAutomationCommand::~FTakeScreenshotAutomationCommand()
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
}

bool FTakeScreenshotAutomationCommand::Update()
{
    if(!ScreenshotRequested)
    {
        const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(HelperFunctions::GetTestWorld(), ScreenshotName, FString{}, Options);
        ScreenshotRequested = true;
    }
    return CommandCompleted;
}






    