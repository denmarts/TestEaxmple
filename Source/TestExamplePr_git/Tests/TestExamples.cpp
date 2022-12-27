// Многие тесты, описанные ниже НЕ БУДУТ пройдены, по причине того, что во многих тестах так же описаны и провальные
// кейсы. Если хочется посмотреть на пройденные тесты закомментируйте провальные кейсы и тестите на здоровье :)

//#if WITH_AUTOMATION_TESTS //Если конфигурация проекта такова, то весь код ниже будет скомпилирован, не знаю как такую конфигурацию внедрить в проект, потому пока закомментирую. Но данная проверка наверняка пригодится в реальном проекте

#include "Misc/AutomationTest.h"
#include "CoreMinimal.h"
#include "DynamicMesh/MeshTransforms.h"
#include "TestExamplePr_git/TestExamplePr_gitCharacter.h"
#include "Tests/AutomationCommon.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "TestExamplePr_git/Tests/Utils/JsonUtils.h"
#include "TestExamplePr_git/Tests/Utils/InputRecordingUtils.h"
#include "GameFramework/PlayerController.h"
#include "HelperFunctions.h"
#include "Components/SplineMovementController.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraActor.h"
#include "ProfilingDebugging/CsvProfiler.h"

// Функции для непосредственного тестирования:
//   TestTrue(TEXT("Некоторое сообщение которое пишется в log если проверка не прошла"), bool <Проверочное предполагаемо логически верное значение>); -> пример использования в тесте "TestTrueExample"
//   TestEqual(TEXT("Некоторое сообщение которое пишется в log если проверка не прошла"), Проверочное число или значение/Результат вычисления, Предполагаемый результат) -> пример использования в тесте "TestEqualExample"
//   TestTrueExpr(bool <Проверочное предполагаемо логически верное значение>) -> Вариант эквивалентный TestTrue, но без сообщения
//   AddInfo(TEXT("Данная функция выводит указанную строку в log при выполнении теста в любом случае"))
//   AddWarning(TEXT("Данная функция выведет эту строку текста в log, при этом пометит результат теста желтым цветом"))
//   AddExpectedError("Строка, что слово в слово описывает ожидаемую ошибку", EAutomationExpectedErrorFlags::<Contains|Exact>, <Сколько раз ошибка встретится в данном тесте>(не обязательный параметр))
//   TestSame("Данная функция выведет эту строку в log если указанные данные ССЫЛАЮТСЯ на одну и ту же область памяти", <объект1>, <объект2>)
//   TestNotSame("Данная функция выведет эту строку в log если указанные данные НЕ ССЫЛАЮТСЯ на одну и ту же область памяти", <объект1>, <объект2>)
//   TestNull(TEXT("Выводит эту строку текста, если указатель != nullptr"), <указатель>)
//   TestNotNull(TEXT("Ситуация обратная функции выше"), <указатель>)

// Вспомогательные макросы при разработке тестов
//   check(<условие>) //Данный макрос проверяет условие, которое передается ему в качестве параметра и если условие ложно, то программа будет прервана. Данный макрос позволяет проверять различные некорректные условия во время разработки, а в релизный билд он не добавляется, не компилируется
//   UE_LOG(<Категория логирования>, <Уровень логирования>, TEXT("Сообщение")) //Выводит в log указанное сообщение с указаным уровнем логирования. Доступные уровни логирования можно посмотреть через ELogVerbosity::

// Макрос для создания простого теста. Параметры -> Имя генерируемого класса (самого теста), Его расположение в Session Frontend, выбранные фильтры/категории для созданного теста
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestTrueExample, "Autotests.TestTrueTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestEqualExample, "Autotests.TestEqualTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestTrueExprExample, "Autotests.TestTrueExprTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestSameTestNotSameExample, "Autotests.SameTestNotSameTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanDoBlueprintAction, "Autotests.ActorCanDoBlueprintAction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJumpInputSimulationTest, "Autotests.JumpInputSimulationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimpleControlInputSimulationTest, "Autotests.SimpleControlInputSimulationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSlightlyMoreComplicatedControlInputSimulationTest, "Autotests.SlightlyMoreComplicatedControlInputSimulationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimpleMouseInputSimulationTest, "Autotests.SimpleMouseInputSimulationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRecordingInputSimulationTest, "Autotests.SimpleRecordingInputSimulationTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSplineMovementSimulation, "Autotests.SplineMovementSimulation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCoupleOfDifferentActorsCanDoThingsSimultaniously, "Autotests.CoupleOfDifferentActorsCanDoThingsSimultaniously", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFPSMeasurmentsTest, "Autotests.FPSMeasurmentsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRAMMeasurmentsTest, "Autotests.RAMMeasurmentsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCSVToolInteraction, "Autotests.CSVToolInteraction", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
// Тестов в одном файле может быть столько сколько нужно, но желательно структурировать их в отдельные файлы ориентируясь на назначения

//В данной категории находятся тесты, которые либо не работают должным образом, либо не доделаны, либо крашат эдитор
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestCheckExample, "Experimental.CheckTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWorldExists, "Experimental.WorldExistsTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestCheckAddWarningAndAddExpectedErrorExample, "Experimental.CheckAddWarningAndAddExpectedErrorTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDataWritingIntoFileTest, "Experimental.DataWritingIntoFileTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

// Подход с использованием COMPLEX_AUTOMATION_TEST отличается от SIMPLE_AUTOMATION_TEST лишь тем, что в нем присутствует вспомогательная функция GetTests()
// Благодаря которой можно формировать массив параметров OutBeautifiedNames (по сути это пути расположения тестов в Session Frontend, как вот этот -> "Experimental.MapsShouldBeLoaded")
// и массив параметров. После чего способен прогнать, по сути, один и тот же тест, но с разными параметрами.
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMapsShouldBeLoaded, "Autotests.MapsShouldBeLoaded", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FTakingInputsFromDifferentFiles, "Autotests.TakingInputsFromDifferentFiles", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

// Подход с использованием Automation Spec. Описанные ниже подход на данный момент считаются более прогрессивными и удобными.

// Макрос DEFINE_SPEC подходит для примитивных ситуаций, в которых группа описанных в спеке тестов не связаны между собой
// некими общими параметрами, что необходимы для их выполнения. Грубо говоря - группа независимых тестов в одной категории.
DEFINE_SPEC(FSimpleTestFuncsIntoOneSpec, "Autotests.SimpleTestFuncsIntoOneSpec", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
DEFINE_SPEC(FScreenshotTests, "Autotests.ScreenshotTests", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

//Макрос BEGIN_DEFINE_SPEC на пару с END_DEFINE_SPEC образуют пространство для объявления глобальных параметров
// (Которые объявляются между данных макросов). Их могут использовать все тесты данной группы.
BEGIN_DEFINE_SPEC(FMovementSimulationTestGroup, "Autotests.MovementSimulationTestGroupSpec", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

    UWorld* World;
    int32 counter = 0;
    const TArray<FString> Maps
    {
        "/Game/Tests/EmptyLevel_Testable",
        "/Game/Tests/EmptyLevel_Testable_ForRun",
        "/Game/Tests/EmptyLevel_Testable_ComplicatedForRun"
    };
    UBlueprint* goldenSphere;
    AActor* goldenSphereComponent;
    ACharacter* Character;

END_DEFINE_SPEC(FMovementSimulationTestGroup)

//Таким образом мы можем создать латентную команду. Нужно разобраться в чем отличие ONE_PARAMETER от всех остальных (это лучше просмотреть когда дойдете до input-тестов)
DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);
bool FJumpLatentCommand::Update()
{
    //if (!Character->InputComponent) return true;
    const int32 ActionIndex = HelperFunctions::GetActionBindingsIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
    if(ActionIndex != -1)
    {
        const auto JumpActionBind = Character->InputComponent->GetActionBinding(ActionIndex);
        JumpActionBind.ActionDelegate.Execute(EKeys::SpaceBar);
    }
    return true;
}

// Тестируем функцию TestTrue()
bool FTestTrueExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestTrueExample is running"));
    TestTrue(TEXT("This test will be passed"), true);
    TestTrue(TEXT("And this one is not. You will see it in the log"), false);
    return true; // данный return в конце требуется оставлять в любом тесте, (Любое false возвращенное любой внутритестовой функцией завалит тест) 
}

// Тестируем функцию TestEqual()
bool FTestEqualExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestEqualExample is running"));
    TestEqual("Expected 25. Test failed", FMath::Max(25, 13), 13); //тест не пройден
    TestEqual("Expected 25", FMath::Max(25, 13), 25); //тест пройден
    return true; 
}

// Тестируем функцию TestTrueExpr()
bool FTestTrueExprExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestTrueExprExample is running"));
    TestTrueExpr( 1 == 1 ); // Единица равняется единице и тест будет пройден успешно
    TestTrueExpr( 1 == 2 ); // Единица не равняется двойке и тест завалится
    return true;
}

// Тестируем функцию check()
bool FTestCheckExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestCheckExample is running"));
    check(false); //на этом моменте вам выкинут exception. Он не фатален, но дает сигнал разрабу, что что-то пошло не так. Можно нажать на Continue и программа пойдет дальше.
    TestTrueExpr(true);
    return true;
}

// Тестируем функции AddWarning() и AddExpectedError()
bool FTestCheckAddWarningAndAddExpectedErrorExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestCheckAddWarningAndAddExpectedErrorExample is running"));
    AddWarning(TEXT("This string will be shown up as a warning into log"));
    AddExpectedError("This string will be shown up as an error into log", EAutomationExpectedErrorFlags::Contains); //Contains сравнивает строку которую мы указали со строкой выданной нам ошибки и если наша строка является ПОДСТРОКОЙ в ошибке, то данная ошибка будет в рантайме проигнорирована
                                                                                                            //Exact - в этом случае строка с описанием ошибки должна полностью совпадать с ошибкой выкинутой нам в рантайме, только тогда выкинутая нам ошибка будет проигнорирована      
    return true;
}

// Тестируем функции TestNull(), TestSame() и TestNotSame()
bool FTestSameTestNotSameExample::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FTestSameTestNotSameExample is running"));
    //создаем 2 объекта типа StaticMeshComponent (мы не открываем карту и по сути ничего с ними не делаем, мы их нигде не увидим, для нас это просто некие абстрактные объекты)
    const UStaticMeshComponent* mesh_1 = NewObject<UStaticMeshComponent>();
    const UStaticMeshComponent* mesh_2 = NewObject<UStaticMeshComponent>();

    //Проверка на корректность инициализации компонентов
    TestNotNull(TEXT("mesh_1 exists"), mesh_1);
    TestNotNull(TEXT("mesh_2 exists"), mesh_2);

    TestSame(TEXT("Expected equal"), mesh_1, mesh_2);
    TestNotSame(TEXT("Expected equal"), mesh_1, mesh_2);

    return true;
}

// Проверяем указатель на созданный World. Его существование
bool FWorldExists::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FWorldExists is running"));
    
    // Открывает карту по заданному relative пути. Достать этот путь можно кликнув правой кнопкой мыши на нужную карту 
    // внутри эдитора и выбрав пункт "Copy reference". Правда изначально путь будет немного не таким, нужно будет
    // убрать приставку World' в начале и в конце убрать .<название карты>'
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable"));

    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exists"), World);

    //латентная комманда, которая позволит сделать паузу перед закрытием игровой карты (параметр это количество секунд)
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));

    //латентная команда закрывающая карту, инициирует выход из текущей игровой сессии
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    
    return true;
}

//Воспроизводим указанную Blueprint функцию у заспавненного персонажа
bool FCharacterCanDoBlueprintAction::RunTest(const FString& Parameters)
{
    //Сохраняю линк на блюпринт нужного мне character (тоже правой кнопной мыши на персонаже внутри эдитора) "Copy reference"
    const char* CharacterBPName = "/Script/Engine.Blueprint'/Game/Tests/AI/Testable_Character_AI.Testable_Character_AI'";
    //Загружаю этот принт
    const UBlueprint* SpawnedCharacterBP = LoadObject<UBlueprint>(nullptr, *FString(CharacterBPName));
    TestNotNull(TEXT("Blueprint exists"), SpawnedCharacterBP); //Проверка на нулевой указатель
    
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable"));

    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exists"), World);

    //задаем базовые координаты для спавна абстрактного актора
    const FTransform InitialTransform{FVector{1840.0f, 320.0f, 140.0f}};
    
    //Спавним актора с указанного блюпринта
    ACharacter* Character = World->SpawnActor<ACharacter>(SpawnedCharacterBP->GeneratedClass, InitialTransform);
    TestNotNull(TEXT("Character exists"), Character); //Проверка на нулевой указатель
    
    FOutputDeviceNull OutputDeviceNull;
    const FString Command = "JumpTest";
    Character->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(10.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);

    return true;
}

//Тест симулирующий прыжок персонажа + программный спавн коллекта (в остальных тестах уже подготовленные карты с коллектами в нужных местах)
bool FJumpInputSimulationTest::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("JumpInputSimulationTest is running"));

    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    
    const FTransform InitialTransform{FVector{820.0f, 260.0f, 320.0f}};
    const AActor* goldenSphereComponent = World->SpawnActor<AActor>(goldenSphere->GeneratedClass, InitialTransform);
    
    TestNotNull(TEXT("Sphere exist"), goldenSphereComponent);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);
    //Проверка на возможность совершения прыжка персонажем
    TestTrueExpr(Character->CanJump());

    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphereComponent));
    UE_LOG(LogTemp, Warning, TEXT("The Class of the sphere is: %s"), *goldenSphereComponent->GetClass()->GetFName().ToString());
    
    //По теме латентных комманд надо еще посмотреть всякое, пока не очень понимаю как они работают
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f)); //Эта латентная команда ждет указанное количество секунд (но работает почему-то не всегда так, как ожидаешь)
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character)); //Самостоятельно созданная латентная комманда для прыжка. Вы можете найти ее вверху документа
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
    {
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphereComponent);
        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
    }, 2.0f)); //Вот это я, пока что, вообще не понимаю как работает и зачем нужно. Но без этого прыжка не происходит.
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    
    return true;
}

//Тест симулирующий бег персонажа по прямой + сбор коллектов
bool FSimpleControlInputSimulationTest::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("ControlInputSimulationTest is running"));

    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    
    const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward"); //"Move Forward / Backward" - это направления движения. Посмотреть уже предусмотренные бинды можно в Settings->Project Settings->Inputs. 
    UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);

    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        //двигает персонажа в указанном направлении. 
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f); //1.0f - это scale данного бинда. Могут быть разными, открывайте конкретный бинд и ищите нужное значение. Для движения вперед scope = 1.0f. Назад, к примеру, будет -1.0f 
    }, [=]()
    {
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    
    return true;
} 

//Тест симулирующий усложненный паттерн движения персонажа, включая прыжки, повороты и т.д. + сбор коллектов
bool FSlightlyMoreComplicatedControlInputSimulationTest::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("ControlInputSimulationTest is running"));

    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ComplicatedForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));

    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), FTransform{FVector{1544.0f, 1739.0f, 1340.0f}});
    //Поворачиаем камеру в нужное нам направление
    Camera->SetActorRotation({-50.0f, -89.0f, 0.0f});
    if(!TestNotNull("Camera exists", Camera))
    {
        return false;
    }

    //Находим PlayerController
    APlayerController* PC = World->GetFirstPlayerController();
    if(!TestNotNull("Controller exists", PC))
    {
        return false;
    }
    //Даем команду, чтобы наблюдение за сценой велось не с камеры со спины персонажа, а с указаной нами камеры  
    PC->SetViewTarget(Camera);

    int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward");
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f);
    }, [=](){},
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Right / Left");
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()    
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f);
    }, [=](){},
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward");
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(-1.0f);
    }, [=](){},
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Right / Left");
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(-1.0f);
    }, [=]()
    {
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    
    return true;
}

// Симуляция поворота курсора мыши
bool FSimpleMouseInputSimulationTest::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Turn Right / Left Mouse");
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f);
    }, [=](){},
    1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(-1.0f);
    }, [=](){},
    1.0f));
    MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Look Up / Down Mouse");
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f);
    }, [=](){},
    1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(-1.0f);
    }, [=](){},
    1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    
    return true;
}

//Чтение зарание записанных input-ов из файла и воспроизведение записанного поведения персонажа
bool FRecordingInputSimulationTest::RunTest(const FString& Parameters)
{
    //Перед тем, как данный тест проводить, требуется внести коррективы в сам проект
    //В первую очередь убедитесь, что у вас в наличии все соответствующие header и .cpp - файлы
    //Это - TestExampleInputRecordComponent.cpp/.h, InputRecordingUtils.h, JsonUtils.cpp/.h (тут они все находятся в папке Tests)
    //Так же, в файл .Build.cs необходимо добавить соответствующие плагины:
    //В PublicDependencyModuleNames.AddRange() должны быть добавлены параметры "Json" и "JsonUtilities"
    //Далее внутри Эдитора необходимо создать кастомный игровой режим. Для этого идём в Project Settings -> Project -> Map and Modes -> Default game mode -> Нажать на плюсик и назвать как захочется.
    //После этого создаем персонажа при игре за которого и будет записываться движение. Можно создать наследника от базового персонажа, можно сдублировать, не суть
    //Далее дважды кликаем на созданного перса, слева будет вкладка Components, в ней кнопка "Add" с плюсиком. Там в поиске вводим Input Recording и подключаем найденный компонент (если его нет проверьте наличие ранее оговореных классов)
    //Далее в World Settings нужно выставить созданный вами ранее Game Mode в параметре "Game Mode Override" и в параметре "Default Pawn Class" выбрать тестового персонажа, к которому вы подключали компонент на прошлом шагу
    //После чего можете просто нажать на кнопку PLAY (На обычный зеленый треугольник) и запись вашего движения пойдёт. Как только вы выйдете из игрового режима, най файл будет записан и сформирован
    //Сформированнный файл будет находится в папке *Root*/Saved/Tests и будет иметь название по следующему паттерну - CharacterTestInput_<дата_записи>_<время_записи>.json
    //Вам необходимо будет вручную перенести нужный записанный файл в папку Tests/Data и переименовать его просто в CharacterTestInput.json
    //После чего в параметре "Default Pawn Class" в "World Settings" снова выбрать любого персонажа без записывающего компонента
    //И только после этого можно запускать данный тест
    
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));

    //В данную константу записывается путь до предполагаемо существующего .json файла по факту, данный файл
    //должен находится в папке Tests/Data
    const FString FileName = HelperFunctions::GetTestDataDir().Append("CharacterTestInput.json");
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TestTrue("Number of spheres at the end should be zero", HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass) == 0);
            return true;
        }));

    return true;
}

//Тут мы формируем кортеж состоящий из имени теста и пути к карте для последующей их передачи в RunTest() и соответственно выполнения тестов 
void FMapsShouldBeLoaded::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    const TArray<TTuple<FString, FString>> Data =
        {
        {"Empty_Map", "/Game/Tests/EmptyLevel_Testable"},
        {"Simple_Run_Map", "/Game/Tests/EmptyLevel_Testable_ForRun"},
        {"Complex_Run_Map", "/Game/Tests/EmptyLevel_Testable_ComplicatedForRun"}
        };

    for(const auto OneTestData : Data)
    {
        OutBeautifiedNames.Add(OneTestData.Key);
        OutTestCommands.Add(OneTestData.Value);
    }
}

// Второй элемент COMPLEX_AUTOMATION_TEST, который и выполняет открытие указанной мапы
bool FMapsShouldBeLoaded::RunTest(const FString& Parameters)
{
    AutomationOpenMap(Parameters);
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//Тут мы формируем массив состоящий из имени теста, пути к карте и имени соответствующего json-файла, в котором хранятся input-data,
//что были специально предзаписаны и кинуты в папку Data с указанными именами для последующей их передачи в RunTest() 
void FTakingInputsFromDifferentFiles::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapPath;
        FString JsonName;
    };

    const TArray<FTestData> TestData =
        {
            {"EmptyMap", "/Game/Tests/EmptyLevel_Testable", "CharacterTestInputEmptyMap.json"},
            {"Simple_Run_Map", "/Game/Tests/EmptyLevel_Testable_ForRun", "CharacterTestInputSimpleRunMap.json"},
            {"Complex_Run_Map", "/Game/Tests/EmptyLevel_Testable_ComplicatedForRun", "CharacterTestInputComplexRunMap.json"}
        };
    
    for(const auto OneTestData: TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        //Если вам требуется передать в тест несколько параметров одновременно, передайте туда сконкатенированную строку с определенным разделителем
        //(у меня это запятая) для того, чтобы внутри самого теста эту строку по этому разделителю распарсить на несколько подстрок
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s"), *OneTestData.MapPath, *OneTestData.JsonName));
    }
}

// Непосредственно сам RunTest() связаный с функцией выше. Для его запуска, как и с обычным Input-тестом, нужно заранее предзаписать
//действия сформировав json файлы (смотри FRecordingInputSimulationTest) для каждой отдельной карты
bool FTakingInputsFromDifferentFiles::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    
    //Вот тут в как раз мы и парсим Parameters на несколько подстрок и сохраняем в указанный массив
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if(!TestTrue("Map name and json params should exist", ParsedParams.Num() == 2))
    {
        return false;
    }
    
    //В качестве параметра для открытия карты передаем первый распарсеный параметр взятый из массива (путь к карте)
    AutomationOpenMap(ParsedParams[0]);
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));

    //В качестве параметра для имени нужного .json файла передаем уже второй распарсеный параметр взятый из массива
    const FString FileName = HelperFunctions::GetTestDataDir().Append(ParsedParams[1]);
    FInputData InputData;
    if (!JsonUtils::ReadInputData(FileName, InputData)) return false;
    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0)) return false;

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller exists", PlayerController)) return false;

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TestTrue("Number of spheres at the end should be zero", HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass) == 0);
            return true;
        }));

    return true;
}

//Для работы данного теста был создан класс SplineMovementController. Делайте блюпринтового наследника данного класса
//Формируйте сплайн внутри данного блюпринта
bool FSplineMovementSimulation::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TEXT("/Game/Tests/SplineMovement/EmptyLevel_Testable_ForJump"));

    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    const char* SplinePath = "/Script/Engine.Blueprint'/Game/Tests/SplineMovement/MSMC.MSMC'";
    const UBlueprint* Spline = LoadObject<UBlueprint>(nullptr, *FString{SplinePath});
    TestNotNull(TEXT("Spline blueprint exist"), Spline);
    
    const char* RobotPath = "/Script/Engine.Blueprint'/Game/Tests/SplineMovement/Empty_Actor.Empty_Actor'";
    const UBlueprint* RobotBP = LoadObject<UBlueprint>(nullptr, *FString{RobotPath});
    TestNotNull(TEXT("Robot blueprint exist"), RobotBP);
    
    const FTransform InitialTransform{FVector{920.0f, 530.0f, 200.0f}};

    //Тут мы задаем массив точек, из которых и будет динамично формироваться сплайн 
    //Данный функционал работает не корректно и его фикс отложен до момента непосредственной необходимости
    TArray<UE::Math::TVector<double>> Points = {
        {440.0f, -560.0f, 280.0f},
        {440.0f, 1090.0f, 280.0f},
        {560.0f, 1090.0f, 280.0f},
        {1000.0f, 2000.0f, 1000.0f}
    };

    //Так как при спавне компонента при помощи обычного World->SpawnActor невозможно инициализировать
    //данные через конструктор используется конструкция World->SpawnActorDeferred на пару с UGameplayStatics::FinishSpawningActor
    //между данными методами можно вызывать функции, которые будут вызваны до спавна самого актора, в том числе
    //функции имитирующие работу конструктора
    ASplineMovementController* SplineComponent = World->SpawnActorDeferred<ASplineMovementController>(Spline->GeneratedClass, InitialTransform);
        //Вызов самодельной функции-конструктора, туда мы передаем UWorld, класс объекта, который будет передвигаться
        //вдоль сплайна, потом будет ли передвижение по сплайну циклично и время, за которое персонаж должен дойти до конца
        SplineComponent->AddParameters(World, *RobotBP->GeneratedClass, false, 10.0f);

        //Функция ниже должна была формировать сплайн динамично. Работает не корректно
        //SplineComponent->MakeYourOwnSpline(Points);
    UGameplayStatics::FinishSpawningActor(SplineComponent, InitialTransform);
    
    TestNotNull(TEXT("Spline exist"), SplineComponent);
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(10.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//Простейший пример использования подхода Automation Spec
void FSimpleTestFuncsIntoOneSpec::Define()
{
    //При помощи данной функции мы декларируем подраздел с указанным в "" именем 
    Describe("BasicTestFuncGroup",
        [this]()
        {
            //При помощи It мы создаем отдельный тест в группе, задаем ему специфическое
            //имя и описываем само тело теста
            It("TestTrueExample", [this]()
            {
                AddInfo(TEXT("FTestTrueExample is running"));
                TestTrue(TEXT("This test will be passed"), true);
                TestTrue(TEXT("And this one is not. You will see it in the log"), false);
            });
            It("TestEqualExample", [this]()
            {
                AddInfo(TEXT("FTestEqualExample is running"));
                TestEqual("Expected 25. Test failed", FMath::Max(25, 13), 13);
                TestEqual("Expected 25", FMath::Max(25, 13), 25); 
            });
            It("TestTrueExprExample", [this]()
            {
                AddInfo(TEXT("FTestTrueExprExample is running"));
                TestTrueExpr( 1 == 1 ); 
                TestTrueExpr( 1 == 2 ); 
            });
            //Если добавить перед Describe или It добавить букву х, то данный тест или группа
            //тестов будет пропущена при выполнении раздела
            xIt("TestSameTestNotSameExample", [this]()
            {
                AddInfo(TEXT("FTestSameTestNotSameExample is running"));
                const UStaticMeshComponent* mesh_1 = NewObject<UStaticMeshComponent>();
                const UStaticMeshComponent* mesh_2 = NewObject<UStaticMeshComponent>();
                
                TestNotNull(TEXT("mesh_1 exists"), mesh_1);
                TestNotNull(TEXT("mesh_2 exists"), mesh_2);

                TestSame(TEXT("Expected equal"), mesh_1, mesh_2);
                TestNotSame(TEXT("Expected equal"), mesh_1, mesh_2);
            });
        });

    //В одном ::Define() может быть одновременно несколько Describe(), что сформирует
    //еще одну группу тестов в указанной категории
    Describe("AnotherTestGroupJustForExample",
        [this]()
        {
            It("EmptyExampleTest1", [this]()
            {
                TestTrue("Something", true);
            });
            It("EmptyExampleTest2", [this]()
            {
                TestTrue("Something else", false);
            });
        });
}

//Более сложный пример использования подхода Automation Spec с использованием BeforeEach() и AfterEach()
void FMovementSimulationTestGroup::Define()
{
    Describe("MovementSimulationTestGroup",
        [this]()
        {
            //Данная функция выполняется перед каждым It() и внутри нее находятся команды, общие для каждого теста
            //Например команда "Открытия карты" используется что для TestJumpExample, что для TestSimpleRunExample
            //Так как она используется во всех двух тестах данной группы, мы их и вынесли в BeforeEach().
            BeforeEach([this]()
            {
                AutomationOpenMap(Maps[counter]);
                World = HelperFunctions::GetTestWorld();
                TestNotNull(TEXT("World exist"), World);
                counter++;

                Character = UGameplayStatics::GetPlayerCharacter(World, 0);
                TestNotNull(TEXT("Character exist"), Character);
                
                const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
                goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
                TestNotNull(TEXT("Blueprint exist"), goldenSphere);
            });
            It("TestJumpExample", [this]()
            {
                const FTransform InitialTransform{FVector{820.0f, 260.0f, 320.0f}};
                goldenSphereComponent = World->SpawnActor<AActor>(goldenSphere->GeneratedClass, InitialTransform);
                TestNotNull(TEXT("Sphere exist"), goldenSphereComponent);

                TestTrueExpr(Character->CanJump());

                UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphereComponent));
                
                ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.0f))
                ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
                ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
                {
                    const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphereComponent);
                    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
                    TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
                }, 2.0f));
            });
            It("TestSimpleRunExample", [this]()
            {
                const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward");
                UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);

                UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));
    
                ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
                ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
                {
                    Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f); 
                }, [=]()
                {
                     const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
                        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
                       TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
                },
                2.0f));
            });

            //Данная функция выполняется после каждого It(), тут по сути то же самое, что и в BeforeEach(), только срабатывает
            //после каждого теста.
            AfterEach([this]()
            {
                ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
            });
        });
}

//Пример использования Screenshot-теста
void FScreenshotTests::Define()
{
    //После запуска данного теста у вас сформируется специальный скриншот, найти его можно
    //в Session Frontend -> Screenshot Comparison (рядом с Automation). Но на данном этапе проводить
    //тест все еще не представляется возможным, так как для того, чтобы скриншот тест
    //работал ему необходим так называемый "Эталонный скриншот" или же "Ground Truth".
    //Для того, чтобы его сделать и закрепить за данным тестом, необходимо его сформировать (первичным запуском теста)
    //Потом подкрепить git к вашему проекту (Делается это путем нажатия на Source control в правом нижнем углу
    //эдитора (но только не во вкладке Session Frontend, а в обычном меню, где у вас карта или около того), там
    //выбираем git (Beta) и указываем путь до гита, если он его самостоятельно не подтянул). После того как вы решите
    //вопрос с Source Control перейдите снова во вкладку где находится ваш скрин, после проделанных действий у вас
    //должна стать активна кнопка "Add New!". Нажимаете и эталонный скриншот для вашего теста будет создан, после чего
    //уже можно проганять данный тест и получать с него реальные результаты.
    Describe("Rendering",
        [this]()
        {
            It("RenderingShouldBeCorrect", [this]()
            {
                AutomationOpenMap("/Game/Tests/EmptyLevel_Testable_ForRun");
                UWorld* World = HelperFunctions::GetTestWorld();
                TestNotNull(TEXT("World exist"), World);

                //Спавним актора камеры и задаем координаты спавна
                ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), FTransform{FVector{1544.0f, 1739.0f, 1340.0f}});
                //Поворачиаем камеру в нужное нам направление
                Camera->SetActorRotation({-25.0f, -89.0f, 0.0f});
                if(!TestNotNull("Camera exists", Camera))
                {
                    return false;
                }

                //Находим PlayerController
                APlayerController* PC = World->GetFirstPlayerController();
                if(!TestNotNull("Controller exists", PC))
                {
                    return false;
                }
                //Даем команду, чтобы наблюдение за сценой велось не с камеры со спины персонажа, а с указаной нами камеры  
                PC->SetViewTarget(Camera);

                //Класс FTakeScreenshotAutomationCommand находится в HelperFunctions.
                //Тут мы делаем скриншот с указанным в параметре именем скриншота
                ADD_LATENT_AUTOMATION_COMMAND(FTakeScreenshotAutomationCommand("rendering_screenshot"));
                ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
                return true;
            });
        });
}

//Тест демонстрирующий возможность спавна одновременно нескольких персонажей для выполнения последующих команд
bool FCoupleOfDifferentActorsCanDoThingsSimultaniously::RunTest(const FString& Parameters)
{
    
    const char* Character_Follow = "/Script/Engine.Blueprint'/Game/Tests/AI/Testable_AI_Character.Testable_AI_Character'";
    const char* Character_Random = "/Script/Engine.Blueprint'/Game/Tests/AI/Testable_AI_Character_V2.Testable_AI_Character_V2'";
    const TArray<UBlueprint*> AICharacters =
        {
            LoadObject<UBlueprint>(nullptr, *FString(Character_Follow)),
            LoadObject<UBlueprint>(nullptr, *FString(Character_Random))
        };
    for(const auto currentCharacter : AICharacters)
    {
        TestNotNull(TEXT("Blueprint exists"), currentCharacter);
    }
    
    //const char* AIControllerRandomLocation = "/Script/Engine.Blueprint'/Game/Tests/AI/Test_AI_Controller_V2.Test_AI_Controller_V2'";
    //const char* AIControllerFollowMe = "/Script/Engine.Blueprint'/Game/Tests/AI/Test_AIController_V1.Test_AIController_V1'";
    /*
    const TArray<UBlueprint*> AIControllers = {
        LoadObject<UBlueprint>(nullptr, *FString(AIControllerRandomLocation)),
        LoadObject<UBlueprint>(nullptr, *FString(AIControllerFollowMe))
    };
    
    */
    
    AutomationOpenMap(TEXT("/Game/Tests/AI/EmptyLevel_AI_Testable"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exists"), World);

    constexpr int32 spawnCharacterCount = 5;
    float spaceBetween = 100.0f;
    for(int i = 0; i < spawnCharacterCount; i++)
    {
        ACharacter* Character = World->SpawnActor<ACharacter>(AICharacters[FMath::RandRange(0, AICharacters.Num() - 1)] ->
            GeneratedClass, FTransform {FVector{1600.0f, -100.0f + spaceBetween, 140.0f}});
        Character->SetActorRotation({0.0f, 180.0f, 0.0f});
        //Character->AIControllerClass = AIControllers[FMath::RandRange(0, 1)]->GeneratedClass;
        //Character->PossessedBy(AIControllers[FMath::RandRange(0, 1)]->GeneratedClass);
        spaceBetween += 100;
    }
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(15.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//Универсальный тест позволяющий фиксировать метрики FPS во время его выполнения (данный тест базируется на тесте FSimpleControlInputSimulationTest)
bool FFPSMeasurmentsTest::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("FPSMeasurmentsTest is running"));

    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    
    const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward"); //"Move Forward / Backward" - это направления движения. Посмотреть уже предусмотренные бинды можно в Settings->Project Settings->Inputs. 
    UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);

    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));

    //Создаем массив для последующей записи туда необходимых метрик
    //По закрытию теста на данный момет эдитор может вылетать, по причине использования raw-поинтера, следует детальнее
    //изучить тему использования smart-поинтеров в контесте Unreal Engine
    TArray<HelperFunctions::multiparam_fps_ram>* LinkedFPS;
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        //Заполняем массив полученными метриками FPS через латентную команду (детали работы функции ищите в HelperFunctions)
        LinkedFPS->Add(HelperFunctions::GetFPSwithCurrentPosition(World, Character));
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f); //1.0f - это scale данного бинда. Могут быть разными, открывайте конкретный бинд и ищите нужное значение. Для движения вперед scope = 1.0f. Назад, к примеру, будет -1.0f 
    }, [=]()
    {
        //Проходимся по массиву и выдаем метрики с привязкой к местоположению персонажа и углу поворота его камеры
        for(const auto currentCompFPS : *LinkedFPS)
        {
            UE_LOG(LogTemp, Warning, TEXT("%f FPS was spotted at %f %f %f position, and camera rotation was %f %f %f"),
                currentCompFPS.FPS_or_RAM, currentCompFPS.PlayerPosition.X, currentCompFPS.PlayerPosition.Y, currentCompFPS.PlayerPosition.Z,
                currentCompFPS.CameraRotation.Pitch, currentCompFPS.CameraRotation.Yaw, currentCompFPS.CameraRotation.Roll);
        }
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
        
        //Частично решает проблему, мануально через delete очистить данный массив поинтеров в контексте латентной команды
        //не удается.
        LinkedFPS->Empty();
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//Windows-only Тест позволяющий фиксировать метрики RAM во время его выполнения (данный тест базируется на тесте FSimpleControlInputSimulationTest)
bool FRAMMeasurmentsTest::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    
    const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward"); //"Move Forward / Backward" - это направления движения. Посмотреть уже предусмотренные бинды можно в Settings->Project Settings->Inputs. 
    UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);

    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));

    //Такая же история как и в тесте выше
    TArray<HelperFunctions::multiparam_fps_ram>* LinkedRAM;
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        LinkedRAM->Add(HelperFunctions::GetRAMwithCurrentPosition(Character));
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f); 
    }, [=]()
    {
        //И тут тоже такая же история как в тесте выше
        for(const auto currentCompRAM : *LinkedRAM)
        {
            UE_LOG(LogTemp, Warning, TEXT("%f RAM usage was spotted at %f %f %f position, and camera rotation was %f %f %f"),
                currentCompRAM.FPS_or_RAM, currentCompRAM.PlayerPosition.X, currentCompRAM.PlayerPosition.Y, currentCompRAM.PlayerPosition.Z,
                currentCompRAM.CameraRotation.Pitch, currentCompRAM.CameraRotation.Yaw, currentCompRAM.CameraRotation.Roll);
        }
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
        //И тут, к сожалению, история такая же как и в тесте выше
        LinkedRAM->Empty();
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//Снятие метрик и генерация CSV-файла с необходимыми метриками
bool FCSVToolInteraction::RunTest(const FString& Parameters)
{
    AddInfo(TEXT("CSVToolInteraction is running"));

    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);

    const char* goldenSpherePath = "Blueprint'/Game/Collectibles/GoldenSphere.GoldenSphere'";
    const UBlueprint* goldenSphere = LoadObject<UBlueprint>(nullptr, *FString(goldenSpherePath));
    TestNotNull(TEXT("Blueprint exist"), goldenSphere);
    
    const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward"); //"Move Forward / Backward" - это направления движения. Посмотреть уже предусмотренные бинды можно в Settings->Project Settings->Inputs. 
    UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);

    UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres before: %i"), HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass));
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
    {
        //Запускаем профайлер перед запуском основного теста
        FCsvProfiler::Get()->BeginCapture();
    }, 1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=]()
    {
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f); //1.0f - это scale данного бинда. Могут быть разными, открывайте конкретный бинд и ищите нужное значение. Для движения вперед scope = 1.0f. Назад, к примеру, будет -1.0f 
    }, [=]()
    {
        const int32 NumberOfTheObjects = HelperFunctions::CountingObjectsOnTheMap(World, goldenSphere->GeneratedClass);
        UE_LOG(LogTemp, Warning, TEXT("Number of golden spheres after: %i"), NumberOfTheObjects);
        TestTrue("Number of the golden spheres should be zero", NumberOfTheObjects == 0);
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([=]()
    {
        //После выполнения основных механик, стопим профайлер. После этого .csv файл будет сохранен по пути
        //Saved/Profiler/CSV/ после чего его необходимо будет переконвертировать в .html формат с графиками и прочим
        //для этого был создан специальный .bat-файл (convert_csv_into_report.bat) в папке devops, который переконвертирует
        //ваш .csv и сохранит его в ту же дерикторию. Только поменяйте используемые пути в config.bat под свой проект.
        FCsvProfiler::Get()->EndCapture();
    }, 1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(5.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//На данный момент не работает, должно записывать подсчитаные ФПС в файл, крашит эдитор при записи элементов в массив
bool FDataWritingIntoFileTest::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TEXT("/Game/Tests/EmptyLevel_Testable_ForRun"));
    const UWorld* World = HelperFunctions::GetTestWorld();
    TestNotNull(TEXT("World exist"), World);
    
    const ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    TestNotNull(TEXT("Character exist"), Character);
    
    const int32 MovingIndex = HelperFunctions::GetAxisBindingsIndexByName(Character->InputComponent, "Move Forward / Backward"); //"Move Forward / Backward" - это направления движения. Посмотреть уже предусмотренные бинды можно в Settings->Project Settings->Inputs. 
    UE_LOG(LogTemp, Warning, TEXT("Current moving index is: %i"), MovingIndex);
    
    TArray<HelperFunctions::multiparam_fps_ram> LinkedFPS;
    
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(3.0f)); 
    ADD_LATENT_AUTOMATION_COMMAND(FTestExampleUntilCommand([=, &LinkedFPS]()
    {
        LinkedFPS.Add(HelperFunctions::GetFPSwithCurrentPosition(World, Character));
        Character->InputComponent->AxisBindings[MovingIndex].AxisDelegate.Execute(1.0f);
    }, [=, &LinkedFPS]()
    {
        for(const auto currentCompFPS : LinkedFPS)
        {
            const FString Data = FString::SanitizeFloat(currentCompFPS.FPS_or_RAM) + " " + FString::SanitizeFloat(currentCompFPS.PlayerPosition.X) + " " +
                FString::SanitizeFloat(currentCompFPS.PlayerPosition.Y) + " " + FString::SanitizeFloat(currentCompFPS.PlayerPosition.Z) + " " +
                    FString::SanitizeFloat(currentCompFPS.CameraRotation.Pitch) + " " + FString::SanitizeFloat(currentCompFPS.CameraRotation.Yaw) + " " +
                        FString::SanitizeFloat(currentCompFPS.CameraRotation.Roll);

                        if(!HelperFunctions::WriteStringIntoFile("FPSMeasurments", Data))
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Some of your data hasn't been written"));
                        }
        }
    },
    2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

//#endif
