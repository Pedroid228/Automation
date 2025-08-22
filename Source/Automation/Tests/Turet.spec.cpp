#include "Misc/AutomationTest.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Test_Utils.h"
#include "Tests/AutomationCommon.h"
#include "Turret/Turret.h"
#include "Turret_Projectile/Turret_Projectile.h"

#ifdef WITH_AUTOMATION_TESTS

DECLARE_LOG_CATEGORY_EXTERN(Turret_Spec_Log, All, All);
DEFINE_LOG_CATEGORY(Turret_Spec_Log);

// Turret_Spec
//------------------------------------------------------------------------------------------------------------
BEGIN_DEFINE_SPEC(Turret_Spec, "Test_Auto.Turret_Spec", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::EngineFilter)
UWorld *World = 0;
ATurret *Turret = 0;
TArray<TTuple<int32, float> > Test_Data;
END_DEFINE_SPEC(Turret_Spec)
//------------------------------------------------------------------------------------------------------------

constexpr const char *BP_TURRET_NAME = "Blueprint'/Game/Automation/Blueprints/Turret/BP_Turret.BP_Turret'";
constexpr const char *BP_TURRET_TESTABLE_NAME = "Blueprint'/Game/Automation/Tests/BP_Turret_Testable.BP_Turret_Testable'";
constexpr const char *GAME_MAP = "World'/Game/Maps/Jump_Test_Map_Too_High.Jump_Test_Map_Too_High'";

//------------------------------------------------------------------------------------------------------------
template<class Object_Class, class Property_Class> Property_Class Get_Property_Value_By_Name(const Object_Class *obj, const FString &property_name)
{
	for (TFieldIterator<FProperty> property_iter(obj->StaticClass() ); property_iter != 0; ++property_iter)
	{
		check(property_iter != 0);
		
		if (property_iter->GetName() == property_name)
		{
			return *property_iter->ContainerPtrToValuePtr<Property_Class>(obj);
		}
	}
	
	UE_LOG(Turret_Spec_Log, Error, TEXT("[%hs] Property %s was not found"), __FUNCTION__, *property_name);
	return Property_Class();
}
//------------------------------------------------------------------------------------------------------------
void Spec_Close_Level(UWorld *world)
{
	check(world != 0);
	
	if (APlayerController *PC = world->GetFirstPlayerController() )
	{
		PC->ConsoleCommand(TEXT("Exit"), false);
	}
}
//------------------------------------------------------------------------------------------------------------
void Turret_Spec::Define()
{
	Describe("Creational", [this]()
	{
		BeforeEach([this]()
			{
				AutomationOpenMap(GAME_MAP);
				World = AsTest_Utils::Get_Game_World();
				TestNotNull(TEXT("World exsists"), World);
			});
 
			It("Turret c++ class can't created", [this]()
			{
				FString expected_warn_message;
				
				expected_warn_message = FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATurret::StaticClass()->GetName() );
				
				AddExpectedError(expected_warn_message, EAutomationExpectedErrorFlags::Exact);
				Turret = World->SpawnActor<ATurret>(ATurret::StaticClass(), FTransform::Identity);
				TestNull(TEXT("Turrent doesn't exists"), Turret);
			});

			It("Turret BP class should be created", [this]()
			{
				Turret = AsTest_Utils::Spawn_BP_Actor<ATurret>(World, BP_TURRET_NAME, FTransform::Identity);
				TestNotNull(TEXT("Turrent exists"), Turret);
			});
 
			AfterEach([this]()
			{
				Spec_Close_Level(World);
			});
	});

	Describe("Default", [this]()
	{
		BeforeEach([this]()
			{
				AutomationOpenMap(GAME_MAP);
				World = AsTest_Utils::Get_Game_World();
				TestNotNull(TEXT("World exsists"), World);
			
				Turret = AsTest_Utils::Spawn_BP_Actor<ATurret>(World, BP_TURRET_TESTABLE_NAME, FTransform::Identity);
				TestNotNull(TEXT("Turret exsists"), Turret);

				Test_Data = { {10, 0.5}, {18, 0.1}, {22, 12.0} };
			});
 
			It("Turret should be setup correctly", [this]()
			{
				int32 start_projectiles_count;
				double fire_frequency;
				TArray<FString> test_data_str;

				for (TTuple<int32, float> &data : Test_Data)
				{
					test_data_str.Empty();
					test_data_str.Add(FString::FromInt(data.Key) );
					test_data_str.Add(FString::SanitizeFloat(data.Value) );
					
					AsTest_Utils::Call_Function_By_Name_With_Params(Turret, TEXT("Event_Test_Setup"), test_data_str);
					
					start_projectiles_count = Get_Property_Value_By_Name<ATurret, int32>(Turret, TEXT("Start_Projectiles_Count") );
					TestTrueExpr(start_projectiles_count == data.Key);
					
					fire_frequency = Get_Property_Value_By_Name<ATurret, double>(Turret, TEXT("Fire_Frequency") );
					TestTrueExpr(FMath::IsNearlyEqual(fire_frequency, data.Value) );
				}
			});
		
			AfterEach([this]()
			{
				Spec_Close_Level(World);
			});
	});


	Describe("Projectiles", [this]()
	{
		LatentBeforeEach([this](const FDoneDelegate &done)
			{
				AutomationOpenMap(GAME_MAP);
				World = AsTest_Utils::Get_Game_World();
				TestNotNull(TEXT("World exsists"), World);
			
				Turret = AsTest_Utils::Spawn_BP_Actor_Deferred<ATurret>(World, BP_TURRET_TESTABLE_NAME, FTransform::Identity);
				TestNotNull(TEXT("Turret exsists"), Turret);
						
				done.Execute();
			});

			Test_Data = { {10, 0.5}, {18, 0.1}, {3, 3.0} };
		
			for (TTuple<int32, float> data : Test_Data)
			{
				double all_fires_time = data.Key * data.Value;
				
				LatentIt(FString::Printf(TEXT("Turret should haven't projectiles after %i sec"), FMath::RoundToInt32(all_fires_time) ), EAsyncExecution::ThreadPool, [this, all_fires_time, data](const FDoneDelegate &done)
				{
					double synch_time_delta = data.Value / 2.0;
					TArray<FString> test_data_str;

					test_data_str.Empty();
					test_data_str.Add(FString::FromInt(data.Key) );
					test_data_str.Add(FString::SanitizeFloat(data.Value) );

					AsyncTask(ENamedThreads::GameThread, [this, test_data_str, data]()
					{
						AsTest_Utils::Call_Function_By_Name_With_Params(Turret, TEXT("Event_Test_Setup"), test_data_str);
						Turret->FinishSpawning(FTransform::Identity);
					});

					FPlatformProcess::Sleep(all_fires_time + synch_time_delta);

					AsyncTask(ENamedThreads::GameThread, [this]()
					{
						int32 curr_projectiles_count = Get_Property_Value_By_Name<ATurret, int32>(Turret, TEXT("Curr_Projectiles_Count") );
						TestTrueExpr(curr_projectiles_count == 0);
					});

					done.Execute();
				});
			}
		
			LatentAfterEach([this](const FDoneDelegate &done)
			{
				Spec_Close_Level(World);
				done.Execute();
			});
	});
}
//------------------------------------------------------------------------------------------------------------
#endif