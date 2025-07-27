#if (WITH_AUTOMATION_TESTS)
#include "Automation/Tests/Inventory_Item_Test.h"
#include "Misc/AutomationTest.h" 
#include "Tests/AutomationCommon.h"
#include "Engine/World.h"
#include "Items/Inventory_Item.h"
#include "Components/TextRenderComponent.h"
#include "Test_Utils.h"
#include "Kismet/GameplayStatics.h"
#include "AutomationCharacter/AutomationCharacter.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCpp_Actor_Can_Be_Spawned, "Items.Inventory.Cpp_Actor_Can_Be_Created", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprint_Actor_Should_Be_Set_Up_Correctly, "Items.Inventory.Blueprint_Actor_Should_Be_Set_Up_Correctly", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventory_Item_Data_Should_Be_Set_Up_Correctly, "Items.Inventory.Inventory_Item_Data_Should_Be_Set_Up_Correctly", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventory_Item_Can_Be_Taken, "Items.Inventory.Inventory_Item_Can_Be_Taken", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventory_Item_Should_Have_Mesh, "Items.Inventory.Inventory_Item_Should_Have_Mesh", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

//------------------------------------------------------------------------------------------------------------
bool FCpp_Actor_Can_Be_Spawned::RunTest(const FString &parameters)
{
	UWorld *world;
	AInventory_Item *item;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	AddExpectedError(TEXT("SpawnActor failed because class Inventory_Item is abstract"), EAutomationExpectedErrorFlags::MatchType::Exact);
	item = world->SpawnActor<AInventory_Item>(AInventory_Item::StaticClass(), FTransform::Identity);
	TestNull(TEXT("Item not exists, abstract class"), item);

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FBlueprint_Actor_Should_Be_Set_Up_Correctly::RunTest(const FString &parameters)
{
	int i;
	int count;
	ECollisionChannel curr_channel;
	FTransform item_transform(FTransform::Identity);
	UWorld *world = 0;
	AInventory_Item *item = 0;
	UEnum *enum_base = 0;
	USphereComponent *collision_comp = 0;
	UStaticMeshComponent *static_mesh_comp = 0;
	UTextRenderComponent *text_render_comp = 0;
	const FString item_bp_name = TEXT("Blueprint'/Game/Automation/Blueprints/BP_Inventory_Item.BP_Inventory_Item'");

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	item_transform.SetLocation(FVector{1000.0} );
	item = AsTest_Utils::Spawn_BP_Actor<AInventory_Item>(world, item_bp_name, item_transform);
	TestNotNull(TEXT("Item exists"), item);

	collision_comp = item->FindComponentByClass<USphereComponent>();
	TestNotNull(TEXT("collision_comp exists"), collision_comp);

	TestTrueExpr(collision_comp->GetUnscaledSphereRadius() >= 30.0);
	TestTrueExpr(collision_comp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);

	static_mesh_comp = item->FindComponentByClass<UStaticMeshComponent>();
	TestNotNull(TEXT("static_mesh_comp exists"), static_mesh_comp);

	text_render_comp = item->FindComponentByClass<UTextRenderComponent>();
	TestNotNull(TEXT("text_render_comp exists"), text_render_comp);

	enum_base = StaticEnum<EInventory_Item_Types>();
	count = enum_base->NumEnums() - 2;

	for (i = 0; i < count; i++)
	{
		curr_channel = (ECollisionChannel)i;
		TestTrueExpr(item->GetComponentsCollisionResponseToChannel(curr_channel) == ECollisionResponse::ECR_Overlap);
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FInventory_Item_Data_Should_Be_Set_Up_Correctly::RunTest(const FString &parameters)
{
	FString command;
	FLinearColor item_color;
	FTransform item_transform;
	AInventory_Item *inv_item = 0;
	UWorld *world = 0;
	UStaticMeshComponent *mesh = 0;
	UMaterialInterface *material = 0;
	UTextRenderComponent *text_render_comp = 0;
	TArray<FString> params;
	const int new_item_type = (int)EInventory_Item_Types::Cube;
	const int new_score = 52;
	const FLinearColor new_color = FLinearColor::Blue;
	const FString testable_item_name = TEXT("Blueprint'/Game/Automation/Tests/BP_Inventory_Item_Testable.BP_Inventory_Item_Testable'");

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	item_transform.SetLocation(FVector{20000.0} );
	inv_item = AsTest_Utils::Spawn_BP_Actor<AInventory_Item>(world, testable_item_name, item_transform);
	TestNotNull(TEXT("inv_item exists"), inv_item);

	params.Add(FString::FromInt(new_item_type) );
	params.Add(FString::FromInt(new_score) );
	params.Add(new_color.ToString() );

	AsTest_Utils::Call_Function_By_Name_With_Params(inv_item, TEXT("Set_Inventory_Data_Event"), params);

	// Score and Item_Type
	TestTrueExpr( (int)inv_item->Item_Type == new_item_type);
	TestTrueExpr(inv_item->Score == new_score);

	// Material
	mesh = inv_item->FindComponentByClass<UStaticMeshComponent>();
	TestNotNull(TEXT("Item Mesh exists"), inv_item);

	material = mesh->GetMaterial(0);
	TestNotNull(TEXT("Item Material exists"), material);

	FHashedMaterialParameterInfo param_info(TEXT("Color") );
	material->GetVectorParameterValue(param_info, item_color);
	TestTrueExpr(new_color == item_color);

	// Text Render
	text_render_comp = inv_item->FindComponentByClass<UTextRenderComponent>();
	TestNotNull(TEXT("Item Text Render exists"), text_render_comp);

	TestTrueExpr(text_render_comp->TextRenderColor == new_color.ToFColor(true) );
	TestTrueExpr(text_render_comp->Text.ToString() == FString::FromInt(new_score) );

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FInventory_Item_Can_Be_Taken::RunTest(const FString &parameters)
{
	FTransform item_transform(FTransform::Identity);
	AInventory_Item *inv_item = 0;
	UWorld *world = 0;
	AAutomationCharacter *auto_character;
	UInventory_Component *inv_comp;
	TArray<FString> params;
	TArray<AActor *> characters;
	TArray<AActor *> items;
	const int new_item_type = (int)EInventory_Item_Types::Cube;
	const int new_score = 52;
	const FLinearColor new_color = FLinearColor::Blue;
	const FString testable_item_name = TEXT("Blueprint'/Game/Automation/Tests/BP_Inventory_Item_Testable.BP_Inventory_Item_Testable'");

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	item_transform.SetLocation(FVector{1000.0} );
	inv_item = AsTest_Utils::Spawn_BP_Actor<AInventory_Item>(world, testable_item_name, item_transform);
	TestNotNull(TEXT("inv_item exists"), inv_item);

	params.Add(FString::FromInt(new_item_type) );
	params.Add(FString::FromInt(new_score) );
	params.Add(new_color.ToString() );

	AsTest_Utils::Call_Function_By_Name_With_Params(inv_item, TEXT("Set_Inventory_Data_Event"), params);

	UGameplayStatics::GetAllActorsOfClass(world, AAutomationCharacter::StaticClass(), characters);
	TestTrueExpr(characters.Num() == 1);

	auto_character = Cast<AAutomationCharacter>(characters[0]);
	TestNotNull(TEXT("auto_character exists"), auto_character);

	auto_character->SetActorLocation(item_transform.GetLocation() );

	inv_comp = auto_character->FindComponentByClass<UInventory_Component>();
	TestNotNull(TEXT("inv_comp exists"), inv_comp);

	TestTrueExpr(inv_comp->Get_Score_By_Type( (EInventory_Item_Types)new_item_type) == new_score);
	TestTrueExpr(!IsValid(inv_item) );

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FInventory_Item_Should_Have_Mesh::RunTest(const FString &parameters)
{
	int i;
	int num;
	UEnum *enum_base;
	FTransform item_transform(FTransform::Identity);
	FString mesh_message;
	AInventory_Item *inv_item = 0;
	UWorld *world = 0;
	UStaticMeshComponent *mesh_comp = 0;
	UStaticMesh *mesh = 0;
	TArray<FString> params;
	const int new_score = 52;
	const FLinearColor new_color = FLinearColor::Blue;
	const FString testable_item_name = TEXT("Blueprint'/Game/Automation/Tests/BP_Inventory_Item_Testable.BP_Inventory_Item_Testable'");

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );

	world = AsTest_Utils::Get_Game_World();
	TestNotNull(TEXT("World exists"), world);

	enum_base = StaticEnum<EInventory_Item_Types>();
	num = enum_base->NumEnums() - 1;

	for (i = 0; i < num; i++)
	{
		mesh_comp = 0;

		item_transform.SetLocation(FVector{100.0 * (i + 1) } );
		inv_item = AsTest_Utils::Spawn_BP_Actor<AInventory_Item>(world, testable_item_name, item_transform);
		TestNotNull(TEXT("inv_item exists"), inv_item);

		params.Add(FString::FromInt(i) );
		params.Add(FString::FromInt(new_score) );
		params.Add(new_color.ToString() );

		AsTest_Utils::Call_Function_By_Name_With_Params(inv_item, TEXT("Set_Inventory_Data_Event"), params);

		mesh_comp = inv_item->FindComponentByClass<UStaticMeshComponent>();

		mesh = mesh_comp->GetStaticMesh();
		mesh_message = FString::Printf(TEXT("Static mesh for %s exists"), *enum_base->GetNameStringByValue(i) );
		if (! TestNotNull(mesh_message, mesh) )
			return false;

		TestNotNull(TEXT("static mesh exists"), mesh);
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
#endif
