#include "Automation/Tests/Character_Movement_Test.h"
#include "Test_Utils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Tests/AutomationCommon.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Misc/AutomationTest.h" 
#include "Items/Inventory_Item.h"
#include "Json_Utils/Json_Utils.h"

extern DEFINE_LOG_CATEGORY(Test_Log);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventory_Item_Pick_Up_On_Jump, "Test_Auto.Character.Inventory_Item_Pick_Up_On_Jump", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventory_Item_Not_Pick_Up_On_Jump_Too_High, "Test_Auto.Character.Inventory_Item_Not_Pick_Up_On_Jump_Too_High", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacter_Should_Pick_Up_All_Items_Recording, "Test_Auto.Character.Character_Should_Pick_Up_All_Items_Recording", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FMaps_Should_Be_Open, "Test_Auto.Maps_Should_Be_Open", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FCharacter_Should_Pick_Up_All_Items_Recording_Many_Maps, "Test_Auto.Character.Character_Should_Pick_Up_All_Items_Recording_Many_Maps", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

const UInputAction *Get_Input_Action_By_Name(const FString &name)
{
	int i;
	FString curr_action_name;
	ACharacter *character = 0;
	UEnhancedInputComponent *enhanced_input = 0;
	UWorld *world = 0;
	const UInputAction *curr_action = 0;

	world = AsTest_Utils::Get_Game_World();
	if (world == 0)
		UE_LOG(Test_Log, Error, TEXT("World was null") );

	character = UGameplayStatics::GetPlayerCharacter(world, 0);
	if (character == 0)
		UE_LOG(Test_Log, Error, TEXT("Character was null") );

	enhanced_input = Cast<UEnhancedInputComponent>(character->InputComponent);
	if (enhanced_input == 0)
		UE_LOG(Test_Log, Error, TEXT("Enhanced input was null") );

	const TArray<TUniquePtr<FEnhancedInputActionEventBinding> > &event_bindings = enhanced_input->GetActionEventBindings();

	for (i = 0; i < event_bindings.Num(); i++)
	{
		if (event_bindings[i] == 0)
			UE_LOG(Test_Log, Error, TEXT("Event binding was null") );

		curr_action = event_bindings[i]->GetAction();
		if (curr_action == 0)
			UE_LOG(Test_Log, Error, TEXT("Curr action was null") );

		curr_action_name = curr_action->GetName();

		if (curr_action_name == name)
			break;
	}

	return curr_action;
}

void Apply_Input(const UInputAction *action, FInputActionValue &action_value, UWorld *world)
{
	UEnhancedPlayerInput *player_input = 0;

	if (world == 0)
	{
		UE_LOG(Test_Log, Error , TEXT(">>>>>>[%hs] World was null"), __FUNCTION__);
		return;
	}

	player_input = AsTest_Utils::Get_Player_Input(world);
	if (player_input == 0)
	{
		UE_LOG(Test_Log, Error , TEXT(">>>>>>[%hs] Player Input was null"), __FUNCTION__);
		return;
	}

	if (action == 0)
	{
		UE_LOG(Test_Log, Error , TEXT(">>>>>>[%hs] Action was null"), __FUNCTION__);
		return;
	}

	const TArray<UInputModifier*> &modifires = action->Modifiers;
	const TArray<UInputTrigger*> &triggers = action->Triggers;

	player_input->InjectInputForAction(action, action_value, modifires, triggers);
}

// FInventory_Item_Pick_Up_On_Jump
//------------------------------------------------------------------------------------------------------------
bool FInventory_Item_Pick_Up_On_Jump::RunTest(const FString &parameters)
{
	const FString jump_action_name = TEXT("IA_Jump");
	UWorld *world = 0;
	const UInputAction *jump_action = 0;
	AActor *inventory_item = 0;
	TArray<AActor *> inventory_items;

	ALevel_Scope level_scope(TEXT("/Game/Maps/Jump_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	if (! TestNotNull(TEXT("World exists"), world) ) return false;

	UGameplayStatics::GetAllActorsOfClass(world, AInventory_Item::StaticClass(), inventory_items);
	TestEqual(TEXT("Have only 1 item on the map"), inventory_items.Num(), 1);
	inventory_item = inventory_items[0];
	TestTrueExpr(IsValid(inventory_item) );

	jump_action = Get_Input_Action_By_Name(jump_action_name);
	if (!TestNotNull(TEXT("Jump action exists"), jump_action) ) return false;

	auto apply_jump = [jump_action, world]()
	{
		FInputActionValue jump_action_value(true);
		Apply_Input(jump_action, jump_action_value, world);
	};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(apply_jump, 2.0f) );

	auto check_item_pick_up = [inventory_item]() 
	{
		if (IsValid(inventory_item) )
		{
			UE_LOG(Test_Log, Error , TEXT("Item was not picked up") );
		}
	};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(check_item_pick_up, 2.0f) );

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FInventory_Item_Not_Pick_Up_On_Jump_Too_High
//------------------------------------------------------------------------------------------------------------
bool FInventory_Item_Not_Pick_Up_On_Jump_Too_High::RunTest(const FString &parameters)
{
	const FString jump_action_name = TEXT("IA_Jump");
	UWorld *world = 0;
	const UInputAction *jump_action = 0;
	AActor *inventory_item = 0;
	TArray<AActor *> inventory_items;

	ALevel_Scope level_scope(TEXT("/Game/Maps/Jump_Test_Map_Too_High") );

	world = AsTest_Utils::Get_Game_World();
	if (! TestNotNull(TEXT("World exists"), world) ) return false;

	UGameplayStatics::GetAllActorsOfClass(world, AInventory_Item::StaticClass(), inventory_items);
	TestEqual(TEXT("Have only 1 item on the map"), inventory_items.Num(), 1);
	inventory_item = inventory_items[0];
	TestTrueExpr(IsValid(inventory_item) );

	jump_action = Get_Input_Action_By_Name(jump_action_name);
	if (!TestNotNull(TEXT("Jump action exists"), jump_action) ) return false;

	auto apply_jump = [jump_action, world]()
	{
		FInputActionValue jump_action_value(true);
		Apply_Input(jump_action, jump_action_value, world);
	};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(apply_jump, 2.0f) );

	auto check_item_pick_up = [inventory_item]() 
	{
		if (! IsValid(inventory_item) )
		{
			UE_LOG(Test_Log, Error , TEXT("Item was picked up") );
		}
	};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(check_item_pick_up, 2.0f) );

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FPlay_Recording_Latent_Command
//------------------------------------------------------------------------------------------------------------
class FPlay_Recording_Latent_Command : public IAutomationLatentCommand
{
public:
	FPlay_Recording_Latent_Command(const FString &file_name, UWorld *world)
	: Index(0), Start_Time(0.0), File_Name(file_name), World(world)
	{
		Json_Utils::Read_Input_Data(File_Name, Input_Data);
	}

	virtual bool Update() override
	{
		double local_time;
		double world_time;
		FInputActionValue curr_action_value;
		ACharacter *player_character;
		const UInputAction *curr_action = 0;

		if (World == 0)
		{
			UE_LOG(Test_Log, Error, TEXT("[%hs] World was null pointer"), __FUNCTION__);
			return true;
		}

		if (! IsValid(World) )
			return true;

		world_time = World->GetTimeSeconds();

		if (Start_Time == 0.0)
		{
			Start_Time = world_time;
			player_character = UGameplayStatics::GetPlayerCharacter(World, 0);

			if (player_character == 0)
			{
				UE_LOG(Test_Log, Error, TEXT("Player Character was null") );
				return true;
			}

			player_character->SetActorTransform(Input_Data.Initial_Transform);
		}

		local_time = world_time - Start_Time;

		// Input
		while (local_time >= Input_Data.Bindings[Index].Time)
		{
			for (FAction_Data &curr_action_data : Input_Data.Bindings[Index].Actions)
			{
				curr_action = curr_action_data.Action;
				curr_action_value = FInputActionValue(curr_action_data.Action_Value);

				if (curr_action == 0)
				{
					UE_LOG(Test_Log, Error, TEXT("Current Action was null") );
					return true;
				}

				Apply_Input(curr_action, curr_action_value, World);
			}

			Index++;

			if (Index >= Input_Data.Bindings.Num() )
				return true;
		}

		return false;
	}

private:
	int Index;
	double Start_Time;
	FString File_Name;
	FInput_Data Input_Data;
	UWorld *World;
};
//------------------------------------------------------------------------------------------------------------

// FCharacter_Should_Pick_Up_All_Items_Recording
//------------------------------------------------------------------------------------------------------------
bool FCharacter_Should_Pick_Up_All_Items_Recording::RunTest(const FString &parameters)
{
	const FString file_name = TEXT("F:/Work/Unreal_Engine/Unreal_Projects/Automation/Source/Automation/Tests/Input_Recording/Character_Should_Pick_Up_All_Items.json");
	UWorld *world = 0;
	TArray<AActor *> inventory_items;

	ALevel_Scope level_scope(TEXT("/Game/Maps/L_Items_Road") );

	world = AsTest_Utils::Get_Game_World();
	if (! TestNotNull(TEXT("World exists"), world) ) return false;

	UGameplayStatics::GetAllActorsOfClass(world, AInventory_Item::StaticClass(), inventory_items);
	for (AActor *inventory_item : inventory_items)
		TestTrueExpr(IsValid(inventory_item) );

	auto check_items_pick_up = [inventory_items]()
		{
			for (AActor *invetory_item : inventory_items)
			{
				if (IsValid(invetory_item) )
					UE_LOG(Test_Log, Error , TEXT("Item was not picked up") );
			}
		};

	//ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5) );
	ADD_LATENT_AUTOMATION_COMMAND(FPlay_Recording_Latent_Command(file_name, world) );
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5) );
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(check_items_pick_up, 2.0f) );  // Check all items picked up

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FMaps_Should_Be_Open
//------------------------------------------------------------------------------------------------------------
void FMaps_Should_Be_Open::GetTests(TArray<FString>& out_beautified_names, TArray <FString>& out_test_commands) const
{
	const TArray<TPair<FString, FString> > map_tests =
	{
		{TEXT("First_Map"), TEXT("/Game/Maps/L_Items_Road")},
		{TEXT("Second_Map"), TEXT("/Game/Maps/L_Items_Road_Custom")}
	};

	for (TPair<FString, FString> map_test : map_tests)
	{
		out_beautified_names.Add(map_test.Key);
		out_test_commands.Add(map_test.Value);
	}
}
//------------------------------------------------------------------------------------------------------------
bool FMaps_Should_Be_Open::RunTest(const FString& parameters)
{
	ALevel_Scope level_scope(parameters);
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f) );
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FCharacter_Should_Pick_Up_All_Items_Recording_Many_Maps
//------------------------------------------------------------------------------------------------------------
void FCharacter_Should_Pick_Up_All_Items_Recording_Many_Maps::GetTests(TArray<FString>& out_beautified_names, TArray <FString>& out_test_commands) const
{
	FString command;

	const TArray<TTuple<FString, FString, FString> > map_tests =
	{
		{TEXT("First_Map"), TEXT("/Game/Maps/L_Items_Road"), TEXT("Character_Should_Pick_Up_All_Items.json")},
		{TEXT("Second_Map"), TEXT("/Game/Maps/L_Items_Road_Custom"), TEXT("Character_Should_Pick_Up_All_Items_Second.json")}
	};

	for (TTuple<FString, FString, FString> map_test : map_tests)
	{
		out_beautified_names.Add(map_test.Get<0>() );

		command = FString::Printf(TEXT("%s,%s"), *map_test.Get<1>(), *map_test.Get<2>() );
		out_test_commands.Add(command);
	}
}
//------------------------------------------------------------------------------------------------------------
bool FCharacter_Should_Pick_Up_All_Items_Recording_Many_Maps::RunTest(const FString& parameters)
{
	FString file_name = TEXT("F:/Work/Unreal_Engine/Unreal_Projects/Automation/Source/Automation/Tests/Input_Recording/");
	UWorld *world = 0;
	TArray<AActor *> inventory_items;
	TArray<FString> params;

	parameters.ParseIntoArray(params, TEXT(",") );

	ALevel_Scope level_scope(params[0]);

	file_name = TEXT("F:/Work/Unreal_Engine/Unreal_Projects/Automation/Source/Automation/Tests/Input_Recording/");
	file_name.Append(params[1]);

	world = AsTest_Utils::Get_Game_World();
	if (! TestNotNull(TEXT("World exists"), world) ) return false;

	UGameplayStatics::GetAllActorsOfClass(world, AInventory_Item::StaticClass(), inventory_items);
	for (AActor *inventory_item : inventory_items)
		TestTrueExpr(IsValid(inventory_item) );

	auto check_items_pick_up = [inventory_items]()
		{
			for (AActor *invetory_item : inventory_items)
			{
				if (IsValid(invetory_item) )
					UE_LOG(Test_Log, Error , TEXT("Item was not picked up") );
			}
		};

	//ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5) );
	ADD_LATENT_AUTOMATION_COMMAND(FPlay_Recording_Latent_Command(file_name, world) );
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5) );
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(check_items_pick_up, 2.0f) );  // Check all items picked up

	return true;
}
//------------------------------------------------------------------------------------------------------------
