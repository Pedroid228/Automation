#include "Input_Recording_Component.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Tests/Test_Utils.h"
#include "EnhancedPlayerInput.h"
#include "Json_Utils/Json_Utils.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(Input_Recording_Log);

// UInput_Recording_Component
//------------------------------------------------------------------------------------------------------------
UInput_Recording_Component::UInput_Recording_Component()
: File_Name(TEXT("Input_Data_Record") )
{
	PrimaryComponentTick.bCanEverTick = true;
}
//------------------------------------------------------------------------------------------------------------
void UInput_Recording_Component::BeginPlay()
{
	UWorld *world = 0;
	AActor *owner = 0;

	world = GetWorld();
	if (world == 0)
		UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] World was null"), __FUNCTION__);

	owner = GetOwner();
	if (owner == 0)
		UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Owner was null"), __FUNCTION__);

	Input_Data.Initial_Transform = owner->GetActorTransform();

	Super::BeginPlay();
}
//------------------------------------------------------------------------------------------------------------
void UInput_Recording_Component::TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction *this_tick_func)
{
	int i;
	FBindings_Data bindings_data;
	FAction_Data action_data;
	UWorld *world = 0;
	ACharacter *owner_character = 0;
	UEnhancedInputComponent *enhanced_input = 0;
	UEnhancedPlayerInput *player_input = 0;
	const UInputAction *curr_action = 0;

	world = GetWorld();
	if (world == 0)
	{
		UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] World was null"), __FUNCTION__);
		return;
	}

	owner_character = Cast<ACharacter>(GetOwner() );
	if (owner_character== 0)
	{
		UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Owner Character was null"), __FUNCTION__);
		return;
	}

	enhanced_input = Cast<UEnhancedInputComponent>(owner_character->InputComponent);
	if (enhanced_input == 0)
	{
		UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Enhanced Input was null"), __FUNCTION__);
		return;
	}

	const TArray<TUniquePtr<FEnhancedInputActionEventBinding> > &event_bindings = enhanced_input->GetActionEventBindings();

	for (i = 0; i < event_bindings.Num(); i++)
	{
		if (event_bindings[i] == 0)
		{
			UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Event Binding was null"), __FUNCTION__);
			return;
		}

		curr_action = event_bindings[i]->GetAction();
		if (curr_action == 0)
		{
			UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Curr Action was null"), __FUNCTION__);
			return;
		}

		player_input = AsTest_Utils::Get_Player_Input(world);
		if (player_input == 0)
		{
			UE_LOG(Input_Recording_Log, Error, TEXT(">>>>>>[%hs] Player Input was null"), __FUNCTION__);
			return;
		}

		action_data.Action = curr_action;
		action_data.Action_Value = (player_input->GetActionValue(curr_action).Get<FVector>() );

		bindings_data.Actions.Add(action_data);
	}

	bindings_data.Time = world->GetTimeSeconds();

	Input_Data.Bindings.Add(bindings_data);

	Super::TickComponent(delta_time, tick_type, this_tick_func);
}
//------------------------------------------------------------------------------------------------------------
void UInput_Recording_Component::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	FString file_path;

	file_path = FPaths::ProjectSavedDir();
	file_path.Append(TEXT("Tests/Input_Recording_Json/") );
	file_path.Append(File_Name).Append(TEXT(".json") );

	/*file_path = TEXT("f:/Work/Unreal_Engine/Unreal_Projects/Automation/Tests");
	file_path.Append(File_Name).Append(TEXT("/") );*/

	Json_Utils::Write_Input_Data(file_path, Input_Data);

	Super::EndPlay(end_play_reason);
}
//------------------------------------------------------------------------------------------------------------
