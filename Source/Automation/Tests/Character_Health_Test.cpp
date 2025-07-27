#include "Automation/Tests/Character_Health_Test.h"
#include "Test_Utils.h"
#include "Tests/AutomationCommon.h"
#include "AutomationCharacter/AutomationCharacter.h"
#include "Misc/AutomationTest.h" 
#include "Engine/DamageEvents.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacter_Can_Be_Damaged, "Test_Auto.Chacter_Health.Character_Can_Be_Damaged", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacter_Can_Be_Killed, "Test_Auto.Chacter_Health.Character_Can_Be_Killed", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacter_Should_Full_Heal, "Test_Auto.Chacter_Health.Character_Should_Full_Heal", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )

namespace BP_Names
{
	FString testable_character_name = TEXT("/Script/Engine.Blueprint'/Game/Automation/Tests/BP_Testable_Character.BP_Testable_Character'");
}

// FCharacter_Can_Be_Damaged
//------------------------------------------------------------------------------------------------------------
bool FCharacter_Can_Be_Damaged::RunTest( const FString &parameters )
{
	const float damage = 20.0f;
	FTransform character_spawn_transform;
	FHealth_Data health_data;
	AAutomationCharacter *character = 0;
	UWorld *world = 0;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	character_spawn_transform.SetLocation(FVector{100.0} );
	character = AsTest_Utils::Spawn_BP_Actor_Deferred<AAutomationCharacter>(world, BP_Names::testable_character_name, character_spawn_transform);
	if (! TestNotNull(TEXT("character exists"), character) )
		return false;

	health_data.Max_Health = 2000.0f;

	AsTest_Utils::Call_Function_By_Name_With_Params(character, TEXT("Set_Health_Data_Event"), {health_data.To_String()});

	character->FinishSpawning(character_spawn_transform);

	TestEqual(TEXT("Health is full"), character->Get_Health_Percent(), 1.0f);
	character->TakeDamage(damage, FDamageEvent{}, 0, 0);
	TestEqual(TEXT("Character was damaged"), character->Get_Health_Percent(), (health_data.Max_Health - damage) / health_data.Max_Health);

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FCheck_Character_Destroy
//------------------------------------------------------------------------------------------------------------
DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCheck_Character_Destroy, float, life_span, ACharacter *, character);
bool FCheck_Character_Destroy::Update()
{
	const double new_timr = FPlatformTime::Seconds();
	if (new_timr - StartTime >= life_span)
	{
		if (!IsValid(character) )
			return true;

		UE_LOG(LogTemp, Error, TEXT("Character was not destroyed on life span end") );
		return false;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------

// FCharacter_Can_Be_Killed
//------------------------------------------------------------------------------------------------------------
bool FCharacter_Can_Be_Killed::RunTest( const FString &parameters )
{
	const float life_span = 4.0f;
	float kill_damage;
	FTransform character_spawn_transform;
	FHealth_Data health_data;
	AAutomationCharacter *character = 0;
	UWorld *world = 0;

	ALevel_Scope level_scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	character_spawn_transform.SetLocation(FVector{100.0} );
	character = AsTest_Utils::Spawn_BP_Actor_Deferred<AAutomationCharacter>(world, BP_Names::testable_character_name, character_spawn_transform);
	if (! TestNotNull(TEXT("character exists"), character) )
		return false;

	health_data.Max_Health = 2000.0f;
	health_data.Life_Span = life_span;

	AsTest_Utils::Call_Function_By_Name_With_Params(character, TEXT("Set_Health_Data_Event"), {health_data.To_String()});

	character->FinishSpawning(character_spawn_transform);

	kill_damage = health_data.Max_Health;
	character->TakeDamage(kill_damage, FDamageEvent{}, 0, 0);
	TestEqual(TEXT("Character was killed"), character->Get_Health_Percent(), 0.0f);

	ADD_LATENT_AUTOMATION_COMMAND(FCheck_Character_Destroy(life_span, character) );

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FCharacter_Can_Be_Killed
//------------------------------------------------------------------------------------------------------------
DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCheck_Character_Full_Heal, float, full_heal_time, AAutomationCharacter *, character);
bool FCheck_Character_Full_Heal::Update()
{
	const double new_time = FPlatformTime::Seconds();
	if (new_time - StartTime >= full_heal_time)
	{
		if (FMath::IsNearlyEqual(character->Get_Health_Percent(), 1.0) )
		{
			UE_LOG(LogTemp, Log, TEXT("Character full healled, percent = %f"), character->Get_Health_Percent() );
			return true;
		}
			

		UE_LOG(LogTemp, Error, TEXT("Character was not full healed for heal time, percent = %f"), character->Get_Health_Percent() );
		return false;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------

// FCharacter_Can_Be_Killed
//------------------------------------------------------------------------------------------------------------
bool FCharacter_Should_Full_Heal::RunTest( const FString &parameters )
{
	float damage;
	float full_heal_time;
	FTransform character_spawn_transform;
	FHealth_Data health_data;
	AAutomationCharacter *character = 0;
	UWorld *world = 0;

	ALevel_Scope level_scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	character_spawn_transform.SetLocation(FVector{100.0} );
	character = AsTest_Utils::Spawn_BP_Actor_Deferred<AAutomationCharacter>(world, BP_Names::testable_character_name, character_spawn_transform);
	if (! TestNotNull(TEXT("character exists"), character) )
		return false;

	health_data.Max_Health = 150.0f;
	health_data.Heal_Per_Sec = 12.0f;
	health_data.Heal_Rate = 0.2f;

	AsTest_Utils::Call_Function_By_Name_With_Params(character, TEXT("Set_Health_Data_Event"), {health_data.To_String()});

	character->FinishSpawning(character_spawn_transform);

	damage = 30.0f;
	character->TakeDamage(damage, FDamageEvent{}, 0, 0);

	full_heal_time = (health_data.Max_Health - (health_data.Max_Health - damage) ) / health_data.Heal_Per_Sec + health_data.Heal_Rate;

	auto check_full_heal = [character]() 
		{
			if (FMath::IsNearlyEqual(character->Get_Health_Percent(), 1.0) )
			{
				UE_LOG(LogTemp, Log, TEXT("Character full healled, percent = %f"), character->Get_Health_Percent() );
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Character was not full healed for heal time, percent = %f"), character->Get_Health_Percent() );
			}
		};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(check_full_heal, full_heal_time) );

	//ADD_LATENT_AUTOMATION_COMMAND(FCheck_Character_Full_Heal(full_heal_time, character) );

	return true;
}
//------------------------------------------------------------------------------------------------------------
