#pragma once
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

class UEnhancedPlayerInput;

DECLARE_LOG_CATEGORY_EXTERN(Test_Log, All, All);

// ALevel_Scope
//------------------------------------------------------------------------------------------------------------
class ALevel_Scope
{
public:
	ALevel_Scope(const FString &map_name);
	~ALevel_Scope();
};
//------------------------------------------------------------------------------------------------------------




// AsTest_Utils
//------------------------------------------------------------------------------------------------------------
class AsTest_Utils
{
public:
	static UWorld *Get_Game_World();
	static void Call_Function_By_Name_With_Params(UObject *object, const FString &func_name, TArray<FString> params);
	static UEnhancedPlayerInput *Get_Player_Input(UWorld *world);
	static void Pause_Pressed(const APlayerController *PC);

	template<typename T> static T* Spawn_BP_Actor(UWorld *world, const FString &bp_name, const FTransform &spawn_transform)
	{
		UBlueprint *blueprint;
		T *actor;

		blueprint = LoadObject<UBlueprint>(0, *bp_name);
		actor = world->SpawnActor<T>(blueprint->GeneratedClass, spawn_transform);

		return actor;
	}

	template<typename T> static T* Spawn_BP_Actor_Deferred(UWorld *world, const FString &bp_name, const FTransform &spawn_transform)
	{
		UBlueprint *blueprint;
		T *actor;

		blueprint = LoadObject<UBlueprint>(0, *bp_name);
		actor = world->SpawnActorDeferred<T>(blueprint->GeneratedClass, spawn_transform);

		return actor;
	}

	template<class T> static T *Get_Widget_By_Class(UWorld *world)
	{
		TArray<UUserWidget *> world_widgets;
	
		if (world == 0)
			UE_LOG(Test_Log, Error, TEXT("[%hs] World was null"), __FUNCTION__);
	
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(world, world_widgets, T::StaticClass() );
		if (world_widgets.Num() != 1)
			return 0;
	
		return Cast<T>(world_widgets[0] );
	}
};
//------------------------------------------------------------------------------------------------------------




// FUntil_Command
//------------------------------------------------------------------------------------------------------------
class FUntil_Command : public IAutomationLatentCommand
{
public:
	FUntil_Command(TFunction<void()> main_callback, TFunction<void()> stop_callback, double timeout = 5.0f);
	virtual bool Update();

private:
	double Timeout;
	TFunction<void()> Main_Callback;
	TFunction<void()> Timeout_Callback;
};
//------------------------------------------------------------------------------------------------------------
