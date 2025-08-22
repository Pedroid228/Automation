#include "Automation/Tests/Screenshot_Tests.h"
#include "Misc/AutomationTest.h" 
#include "AutomationBlueprintFunctionLibrary.h"
#include "Test_Utils.h"
#include "Tests/AutomationCommon.h"
#include "BufferVisualizationData.h"
#include "Engine/DamageEvents.h"
#include "User_Graphics_Settings/Widgets/Pause_Menu_Widget/Pause_Menu_Widget.h"
#include "User_Graphics_Settings/Automation_HUD/Automation_HUD.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRendering_Should_Be_Corrected, "Test_Auto.Screenshots.Rendering_Should_Be_Corrected", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDefault_HUD_Should_Be_Corrected, "Test_Auto.Screenshots.Default_HUD_Should_Be_Corrected", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealth_Bar_Should_Be_Corrected_On_Damage, "Test_Auto.Screenshots.Health_Bar_Should_Be_Corrected_On_Damage", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVideo_Settings_Should_Be_Corrected, "Test_Auto.Screenshots.Video_Settings_Should_Be_Corrected", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

DECLARE_LOG_CATEGORY_EXTERN(FScreenshot_Tests_Log, All, All)
DEFINE_LOG_CATEGORY(FScreenshot_Tests_Log)

class FTake_Game_Rendering_Screenshot : public IAutomationLatentCommand
{
public:
	FTake_Game_Rendering_Screenshot(UWorld *world)
		: Is_Screenshot_Requested(false), Is_Screenshot_Taken(false), World(world)
	{
		Screenshot_Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForGameplay();
		Screenshot_Options.VisualizeBuffer = (TEXT("SceneDepth") );
		FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTake_Game_Rendering_Screenshot::On_Screenshot_Taken_And_Compared);
	}

	virtual bool Update() override
	{
		if (!Is_Screenshot_Requested)
		{
			if (World == 0)
			{
				UE_LOG(FScreenshot_Tests_Log, Error, TEXT("[%hs] World was null"), __FUNCTION__);
				return true;
			}
			
			UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(World, TEXT("Rendering_Should_Be_Corrected_Screenshot"), TEXT(""), Screenshot_Options);
			Is_Screenshot_Requested = true;
		}
		
		return Is_Screenshot_Taken;
	}

private:
	UFUNCTION() void On_Screenshot_Taken_And_Compared()
	{
		Is_Screenshot_Taken = true;
	}
	
	bool Is_Screenshot_Requested;
	bool Is_Screenshot_Taken;
	FAutomationScreenshotOptions Screenshot_Options;
	UWorld *World;
};
//------------------------------------------------------------------------------------------------------------




// FTake_UI_Screenshot_Latent_Command
//------------------------------------------------------------------------------------------------------------
class FTake_UI_Screenshot_Latent_Command: public IAutomationLatentCommand
{
public:
	FTake_UI_Screenshot_Latent_Command(UWorld *world, const FString &screenshot_name, double delay = 1.0)
		: Is_Screenshot_Requested(false), Is_Screenshot_Taken(false), Screenshot_Name(screenshot_name), World(world)
	{
		check(World);
		
		Screenshot_Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForGameplay();
		//Screenshot_Options.VisualizeBuffer = (TEXT("Opacity") );
		FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTake_UI_Screenshot_Latent_Command::On_Screenshot_Taken_And_Compared);
		
		Screenshot_Time = World->GetTimeSeconds() + delay;
	}

	virtual bool Update() override
	{
		double curr_time = World->GetTimeSeconds();
		
		if (!Is_Buffer_Setup_Done)
		{
			Set_Visualize_Buffer( TEXT("Opacity") );
			Is_Buffer_Setup_Done = true;
		}

		if (curr_time >= Screenshot_Time)
		{
			if (!Is_Screenshot_Requested)
			{
				if (World == 0)
				{
					UE_LOG(FScreenshot_Tests_Log, Error, TEXT("[%hs] World was null"), __FUNCTION__);
					return true;
				}
				UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(World, Screenshot_Name, Screenshot_Options);
				Is_Screenshot_Requested = true;
			}
		}
		
		return Is_Screenshot_Taken;
	}

private:
	UFUNCTION() void On_Screenshot_Taken_And_Compared()
	{
		Is_Screenshot_Taken = true;
		Set_Visualize_Buffer(NAME_None);  // Reset
	}

	void Set_Visualize_Buffer(const FName &buffer_name)
	{
		if (UGameViewportClient* ViewportClient = AutomationCommon::GetAnyGameViewportClient())
		{
			static IConsoleVariable* ICVar = IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
			if (ICVar)
			{
				if (ViewportClient->GetEngineShowFlags())
				{
					ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(buffer_name == NAME_None ? false : true);
					ViewportClient->GetEngineShowFlags()->SetTonemapper(buffer_name == NAME_None ? true : false);
					ICVar->Set(buffer_name == NAME_None ? TEXT("") : *buffer_name.ToString() );
				}
			}
		}
	}
	
	bool Is_Screenshot_Requested;
	bool Is_Screenshot_Taken;
	bool Is_Buffer_Setup_Done;
	double Screenshot_Time;
	FString Screenshot_Name;
	FAutomationScreenshotOptions Screenshot_Options;
	UWorld *World;
};
//------------------------------------------------------------------------------------------------------------




// FRendering_Should_Be_Corrected
//------------------------------------------------------------------------------------------------------------
bool FRendering_Should_Be_Corrected::RunTest(const FString &parameters)
{
	UWorld *world = 0;
	ALevel_Scope level_scope = ALevel_Scope(TEXT("/Game/Maps/L_Main") );

	world = AsTest_Utils::Get_Game_World();
	if (!TestNotNull(TEXT("World exists"), world) )
		return false;

	ADD_LATENT_AUTOMATION_COMMAND(FTake_Game_Rendering_Screenshot(world))
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FDefault_HUD_Should_Be_Corrected
//------------------------------------------------------------------------------------------------------------
bool FDefault_HUD_Should_Be_Corrected::RunTest(const FString &parameters)
{
	UWorld *world = 0;
	ALevel_Scope level_scope = ALevel_Scope(TEXT("/Game/Maps/L_Main") );

	world = AsTest_Utils::Get_Game_World();
	if (!TestNotNull(TEXT("World exists"), world) )
		return false;

	ADD_LATENT_AUTOMATION_COMMAND(FTake_UI_Screenshot_Latent_Command(world, TEXT("Default_HUD_Screenshot"), 2.0) );
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FHealth_Bar_Should_Be_Corrected_On_Damage
//------------------------------------------------------------------------------------------------------------
bool FHealth_Bar_Should_Be_Corrected_On_Damage::RunTest(const FString &parameters)
{
	APawn *player_pawn;
	UWorld *world = 0;
	ALevel_Scope level_scope = ALevel_Scope(TEXT("/Game/Maps/L_Main") );

	world = AsTest_Utils::Get_Game_World();
	if (!TestNotNull(TEXT("World exists"), world) )
		return false;

	player_pawn = world->GetFirstPlayerController()->GetPawn();
	check(player_pawn != 0);

	player_pawn->TakeDamage(40.0f, FDamageEvent{}, 0, 0);

	ADD_LATENT_AUTOMATION_COMMAND(FTake_UI_Screenshot_Latent_Command(world, TEXT("Health_Bar_On_Damage_Screenshot"), 2.0) );
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FVideo_Settings_Should_Be_Corrected
//------------------------------------------------------------------------------------------------------------
bool FVideo_Settings_Should_Be_Corrected::RunTest(const FString &parameters)
{
	AAutomation_HUD *HUD = 0;
	UPause_Menu_Widget* pause_menu_widget = 0;
	UWorld *world = 0;
	ALevel_Scope level_scope = ALevel_Scope(TEXT("/Game/Maps/L_Main") );

	world = AsTest_Utils::Get_Game_World();
	if (!TestNotNull(TEXT("World exists"), world) )
		return false;

	HUD = Cast<AAutomation_HUD>( world->GetFirstPlayerController()->GetHUD() );
	check(HUD != 0);

	pause_menu_widget = HUD->Get_Pause_Menu_Widget();
	check(pause_menu_widget != 0);

	pause_menu_widget->SetVisibility(ESlateVisibility::Visible);

	ADD_LATENT_AUTOMATION_COMMAND(FTake_UI_Screenshot_Latent_Command(world, TEXT("Video_Settings_Screenshot"), 2.0) );
	
	return true;
}
//------------------------------------------------------------------------------------------------------------
