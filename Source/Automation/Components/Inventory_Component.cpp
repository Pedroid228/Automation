#include "Inventory_Component.h"

// AInventory_Component
//------------------------------------------------------------------------------------------------------------
UInventory_Component::UInventory_Component()
{
}
//------------------------------------------------------------------------------------------------------------
bool UInventory_Component::Try_To_Add_Item(AInventory_Item *new_item)
{
	int new_item_score;
	int new_score;
	int score_limit;
	EInventory_Item_Types new_item_type;

	if (new_item == 0)
		return false;

	new_item_score = new_item->Score;
	new_item_type = new_item->Item_Type;
	score_limit = Score_Limits[new_item_type];

	if (new_item_score < 0)
		return false;

	if (! Inventory.Contains(new_item_type) )  // Is first item of this type in inventory
	{
		if (new_item_score > score_limit)  // Is new item score greater than limit
			return false;

		Inventory.Add(new_item_type, 0);
	}

	new_score = Inventory[new_item_type] + new_item_score;

	if (new_score > score_limit)
		return false;

	Inventory[new_item_type] += new_item_score;

	return true;
}
//------------------------------------------------------------------------------------------------------------
int UInventory_Component::Get_Score_By_Type(EInventory_Item_Types item_type)
{
	int score;

	if (!Inventory.Contains(item_type) )
		return 0;

	score = Inventory[item_type];
	return score;
}
//------------------------------------------------------------------------------------------------------------
void UInventory_Component::BeginPlay()
{
	int i;
	int count;
	EInventory_Item_Types curr_item_type;
	FString curr_item_type_name;

	UEnum *enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 1;

	for (i = 0; i < count; i++)
	{
		curr_item_type = (EInventory_Item_Types)i;
		curr_item_type_name = enum_base->GetNameStringByIndex(i);

		if (! Score_Limits.Contains(curr_item_type) )
			UE_LOG(LogTemp, Warning, TEXT("%s is not added to Score_Limits or less then zero"), *curr_item_type_name);
	}

	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------