#if (WITH_AUTOMATION_TESTS)
#include "Automation/Tests/Inventory_Component_Test.h"
#include "Misc/AutomationTest.h" 
#include "../Components/Inventory_Component.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponent_Could_Be_Created, "Test_Auto.Inventory_Component.Create", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScores_Should_Be_Zero_By_Default, "Test_Auto.Inventory_Component.Scores_Zero_By_Defalt", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositive_Score_Should_Be_Added, "Test_Auto.Inventory_Component.Positive_Score_Should_Be_Added", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegative_Score_Shouldnt_Be_Added, "Test_Auto.Inventory_Component.Negative_Score_Shouldnt_Be_Added", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScore_More_Than_Limit_Cant_Be_Added, "Test_Auto.Inventory_Component.Score_More_Than_Limit_Cant_Be_Added", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )

// UInventory_Component_For_Test
//------------------------------------------------------------------------------------------------------------
class AInventory_Item_For_Test : public UInventory_Component
{
public:
	void Set_Score_Limits(int new_score_limits)
	{
		int i;
		int count;
		EInventory_Item_Types curr_item_type;
		UEnum *enum_base;

		if (new_score_limits < 0)
			return;

		enum_base = StaticEnum<EInventory_Item_Types>();
		count = enum_base->NumEnums() - 1;

		for (i = 0; i < count; i++)
		{
			curr_item_type = (EInventory_Item_Types)i;

			if (! Score_Limits.Contains(curr_item_type) )  // Is first limit of this type in inventory
				Score_Limits.Add(curr_item_type, new_score_limits);
			else
				Score_Limits[curr_item_type] = new_score_limits;
		}
	}
};
//------------------------------------------------------------------------------------------------------------




// FComponent_Could_Be_Created
//------------------------------------------------------------------------------------------------------------
bool FComponent_Could_Be_Created::RunTest( const FString &parameters )
{
	UInventory_Component *inventory_comp = NewObject<UInventory_Component>();
	if (! TestNotNull("inventory_comp exsists", inventory_comp) )
		return false;

	return true;
}
//------------------------------------------------------------------------------------------------------------




// FScores_Should_Be_Zero_By_Default
//------------------------------------------------------------------------------------------------------------
bool FScores_Should_Be_Zero_By_Default::RunTest( const FString &parameters )
{
	int i;
	int count;
	EInventory_Item_Types curr_item_type;
	UInventory_Component *inventory_comp;
	UEnum *enum_base;

	inventory_comp = NewObject<UInventory_Component>();
	enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 1;

	for (i = 0; i < count; i++)
	{
		curr_item_type = (EInventory_Item_Types)i;
		
		if (inventory_comp->Get_Score_By_Type(curr_item_type) != 0)
			return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------

// FPositive_Score_Should_Be_Added
//------------------------------------------------------------------------------------------------------------
bool FPositive_Score_Should_Be_Added::RunTest( const FString &parameters )
{
	int i;
	int count;
	const int positive_item_score = 25;
	const int default_item_score = 16;
	const int score_limit = default_item_score + positive_item_score; 
	EInventory_Item_Types curr_item_type;
	AInventory_Item_For_Test *inventory_comp;
	UEnum *enum_base;

	inventory_comp = NewObject<AInventory_Item_For_Test>();
	inventory_comp->Set_Score_Limits(score_limit);
	enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 1;

	for (i = 0; i < count; i++)
	{
		curr_item_type = (EInventory_Item_Types)i;

		AInventory_Item *inventory_item = NewObject<AInventory_Item>();
		inventory_item->Item_Type = curr_item_type;

		// Set default value
		inventory_item->Score = default_item_score;
		inventory_comp->Try_To_Add_Item(inventory_item);

		// Add positive value
		inventory_item->Score = positive_item_score;
		inventory_comp->Try_To_Add_Item(inventory_item);

		if (inventory_comp->Get_Score_By_Type(curr_item_type) != default_item_score + positive_item_score)
			return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------

// FNegative_Score_Shouldnt_Be_Added
//------------------------------------------------------------------------------------------------------------
bool FNegative_Score_Shouldnt_Be_Added::RunTest( const FString &parameters )
{
	int i;
	int count;
	const int negative_item_score = -12;
	const int default_item_score = 52;
	const int score_limit = default_item_score;
	EInventory_Item_Types curr_item_type;
	AInventory_Item_For_Test *inventory_comp;
	UEnum *enum_base;

	inventory_comp = NewObject<AInventory_Item_For_Test>();
	inventory_comp->Set_Score_Limits(score_limit);
	enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 1;

	for (i = 0; i < count; i++)
	{
		curr_item_type = (EInventory_Item_Types)i;

		AInventory_Item *inventory_item = NewObject<AInventory_Item>();
		inventory_item->Item_Type = curr_item_type;

		// Set default positive value
		inventory_item->Score = default_item_score;
		inventory_comp->Try_To_Add_Item(inventory_item);

		// Add negative value
		inventory_item->Score = negative_item_score;
		inventory_comp->Try_To_Add_Item(inventory_item);

		if (inventory_comp->Get_Score_By_Type(curr_item_type) != default_item_score)  // if negative value, than score not changing => score == default_value
			return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------

// FScore_More_Than_Limit_Cant_Be_Added
//------------------------------------------------------------------------------------------------------------
bool FScore_More_Than_Limit_Cant_Be_Added::RunTest( const FString &parameters )
{
	int i;
	int count;
	const int default_item_score = 52;
	const int score_limit = 55;
	const int max_additional_score_value = score_limit - default_item_score;
	const int transcendental_score_value = max_additional_score_value + 1;
	EInventory_Item_Types curr_item_type;
	AInventory_Item_For_Test *inventory_comp;
	UEnum *enum_base;

	if (default_item_score > score_limit)
		return false;

	inventory_comp = NewObject<AInventory_Item_For_Test>();
	inventory_comp->Set_Score_Limits(score_limit);
	enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 1;

	for (i = 0; i < count; i++)
	{
		curr_item_type = (EInventory_Item_Types)i;

		AInventory_Item *inventory_item = NewObject<AInventory_Item>();
		inventory_item->Item_Type = curr_item_type;

		// Set default positive value
		inventory_item->Score = default_item_score;
		inventory_comp->Try_To_Add_Item(inventory_item);

		// Add transcendental score value
		inventory_item->Score = transcendental_score_value;
		inventory_comp->Try_To_Add_Item(inventory_item);

		if (inventory_comp->Get_Score_By_Type(curr_item_type) != default_item_score)  // if transcendental value, than score not changing => score == default_value
			return false;

		// Add limit score value
		inventory_item->Score = max_additional_score_value;
		inventory_comp->Try_To_Add_Item(inventory_item);

		if (inventory_comp->Get_Score_By_Type(curr_item_type) != score_limit)
			return false;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
#endif
