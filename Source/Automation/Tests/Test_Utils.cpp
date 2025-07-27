#include "Test_Utils.h"
#include "Misc/AutomationTest.h" 
#include "Tests/AutomationCommon.h"
#include "Misc/OutputDeviceNull.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(Test_Log);

// ALevel_Scope
//------------------------------------------------------------------------------------------------------------
ALevel_Scope::ALevel_Scope(const FString &map_name)
{
	AutomationOpenMap(map_name);
}
//------------------------------------------------------------------------------------------------------------
ALevel_Scope::~ALevel_Scope()
{
	ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
}
//------------------------------------------------------------------------------------------------------------




// AsTest_Utils
//------------------------------------------------------------------------------------------------------------
UWorld *AsTest_Utils::Get_Game_World()
{
	const TIndirectArray<FWorldContext> &world_contexts = GEngine->GetWorldContexts();

	for (const FWorldContext &context : world_contexts)
	{
		if ( (context.WorldType == EWorldType::PIE || context.WorldType == EWorldType::Game) && (context.World() != 0) )
		{
			if (UWorld *world = context.World() )
				return world;
			else
				UE_LOG(LogTemp, Error, TEXT("[%hs] world is null pointer"), __FUNCTION__);
		}
	}

	return 0;
}
//------------------------------------------------------------------------------------------------------------
void AsTest_Utils::Call_Function_By_Name_With_Params(UObject *object, const FString &func_name, TArray<FString> params)
{
	FString command;
	FOutputDeviceNull device_null;

	if (object == 0)
		UE_LOG(LogTemp, Fatal, TEXT("[%hs] object was null"), __FUNCTION__);

	command = func_name;
	for (FString &param : params)
	{
		command.Append(TEXT(" ") );
		command.Append(param);
	}

	object->CallFunctionByNameWithArguments(*command, device_null, 0, true);
}
//------------------------------------------------------------------------------------------------------------
UEnhancedPlayerInput *AsTest_Utils::Get_Player_Input(UWorld *world)
{
	ULocalPlayer *local_player = 0;
	UEnhancedPlayerInput *player_input = 0;
	UEnhancedInputLocalPlayerSubsystem *subsystem = 0;
	APlayerController *player_controller = 0;

	if (world == 0)
	{
		UE_LOG(Test_Log, Error, TEXT(">>>>>>[%hs] World was null"), __FUNCTION__);
		return 0;
	}

	player_controller = UGameplayStatics::GetPlayerController(world, 0);
	if (player_controller == 0)
	{
		UE_LOG(Test_Log, Error, TEXT(">>>>>>[%hs] Player Controller was null"), __FUNCTION__);
		return 0;
	}

	local_player = player_controller->GetLocalPlayer();
	if (local_player == 0)
	{
		UE_LOG(Test_Log, Error, TEXT(">>>>>>[%hs] Local player was null"), __FUNCTION__);
		return 0;
	}

	subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(local_player);
	if (subsystem == 0)
	{
		UE_LOG(Test_Log, Error, TEXT(">>>>>>[%hs] Subsystem was null"), __FUNCTION__);
		return 0;
	}

	player_input = subsystem->GetPlayerInput();
	if (player_input == 0)
	{
		UE_LOG(Test_Log, Error, TEXT(">>>>>>[%hs] Player input was null"), __FUNCTION__);
		return 0;
	}

	return player_input;
}
//------------------------------------------------------------------------------------------------------------
void AsTest_Utils::Pause_Pressed(const APlayerController* PC)
{
	int i;
	
	if (PC == 0)
		UE_LOG(Test_Log, Error, TEXT(">>>>>[%hs] PC was null"), __FUNCTION__);
	
	if (PC->InputComponent == 0)
		UE_LOG(Test_Log, Error, TEXT(">>>>>[%hs] PC was null"), __FUNCTION__);

	for (i = 0; i < PC->InputComponent->GetNumActionBindings(); i++)
	{
		FInputActionBinding curr_action_binding = PC->InputComponent->GetActionBinding(i);
		if (curr_action_binding.GetActionName() == TEXT("Toggle_Game_Pause") )
		{
			curr_action_binding.ActionDelegate.Execute(EKeys::P);
			return;
		}
	}

	UE_LOG(Test_Log, Error, TEXT("[%hs] Action name was not found"), __FUNCTION__);
}
//------------------------------------------------------------------------------------------------------------



// FUntil_Command
//------------------------------------------------------------------------------------------------------------
FUntil_Command::FUntil_Command(TFunction<void()> main_call_back, TFunction<void()> timeout_callback, double timeout)
: Timeout(timeout), Main_Callback(MoveTemp(main_call_back)), Timeout_Callback(MoveTemp(timeout_callback) )
{
}
//------------------------------------------------------------------------------------------------------------
bool FUntil_Command::Update()
{
	double time_runnig = GetCurrentRunTime();

	if (time_runnig >= Timeout)
	{
		Timeout_Callback();
		return true;
	}
	else
	{
		Main_Callback();
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------
