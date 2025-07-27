#include "Automation/Tests/UI_Tests.h"
#include "Misc/AutomationTest.h" 
#include "Test_Utils.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "User_Graphics_Settings/Widgets/Pause_Menu_Widget/Pause_Menu_Widget.h"
#include "Components/VerticalBox.h"
#include "User_Graphics_Settings/Widgets/Video_Settings_Widget/Video_Settings_Widget.h"
#include "User_Graphics_Settings/Widgets/Setting_Option_Widget/Setting_Option_Widget.h"
#include "Components/Button.h"
#include "User_Graphics_Settings/Game_User_Settings/Game_User_Settings.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPause_Menu_Should_Be_Change_Visibility_On_Pause, "Test_Auto.UI.Pause_Menu_Should_Be_Change_Visibility_On_Pause", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAll_Video_Settings_Exist, "Test_Auto.UI.All_Video_Settings_Exist", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVideo_Setting_Can_Be_Applied, "Test_Auto.UI.Video_Setting_Can_Be_Applied", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRun_Benchmark, "Test_Auto.UI.Run_Benchmark", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::MediumPriority | EAutomationTestFlags::StressFilter)



// FPause_Menu_Should_Be_Visible_On_Pause
//------------------------------------------------------------------------------------------------------------
bool FPause_Menu_Should_Be_Change_Visibility_On_Pause::RunTest(const FString &parameters)
{
	UWorld *world = 0;
	APlayerController *PC = 0;
	UPause_Menu_Widget *pause_menu_widget = 0;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;

	PC = world->GetFirstPlayerController();
	if (!TestTrueExpr(PC != 0) )
		return false;

	pause_menu_widget = AsTest_Utils::Get_Widget_By_Class<UPause_Menu_Widget>(world);
	if (!TestTrueExpr(pause_menu_widget != 0) )
		return false;

	TestTrueExpr(pause_menu_widget->GetVisibility() == ESlateVisibility::Collapsed);
	AsTest_Utils::Pause_Pressed(PC);
	TestTrueExpr(pause_menu_widget->GetVisibility() == ESlateVisibility::Visible);
	AsTest_Utils::Pause_Pressed(PC);
	TestTrueExpr(pause_menu_widget->GetVisibility() == ESlateVisibility::Collapsed);
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FAll_Video_Settings_Exist
//------------------------------------------------------------------------------------------------------------
bool FAll_Video_Settings_Exist::RunTest(const FString &parameters)
{
	int children_count = 0;
	UWorld *world = 0;
	UPause_Menu_Widget *pause_menu_widget = 0;
	UVideo_Settings_Widget *video_settings_widget = 0;
	UVerticalBox *container = 0;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;

	pause_menu_widget = AsTest_Utils::Get_Widget_By_Class<UPause_Menu_Widget>(world);
	if (!TestTrueExpr(pause_menu_widget != 0) )
		return false;

	// video_settings_widget = Cast<UVideo_Settings_Widget>(pause_menu_widget->WidgetTree->FindWidget(TEXT("Video_Settings") ) );
	// if (!TestTrueExpr(video_settings_widget != 0) )
	// 	return false;
	//
	// container = Cast<UVerticalBox>(video_settings_widget->WidgetTree->FindWidget(TEXT("Container") ) );
	// if (!TestTrueExpr(container != 0) )
	// 	return false;

	auto predicate = [&container](UWidget *widget)
	{
		if (widget->GetName() == TEXT("Container") )
			container = Cast<UVerticalBox>(widget);
	};
	
	pause_menu_widget->WidgetTree->ForEachWidgetAndDescendants(predicate);
	if (!TestTrueExpr(container != 0) )
		return false;
	
	children_count = container->GetChildrenCount();
	if (!TestTrueExpr(children_count == 7) )
		return false;
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FVideo_Setting_Can_Be_Applied
//------------------------------------------------------------------------------------------------------------
bool FVideo_Setting_Can_Be_Applied::RunTest(const FString &parameters)
{
	int default_GI_quality;
	UWorld *world = 0;
	UPause_Menu_Widget *pause_menu_widget = 0;
	UVideo_Settings_Widget *video_settings_widget = 0;
	UVerticalBox *container = 0;
	USetting_Option_Widget *GI_setting;
	UButton *right_button = 0;
	UGameUserSettings *game_user_settings = 0;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;
	
	pause_menu_widget = AsTest_Utils::Get_Widget_By_Class<UPause_Menu_Widget>(world);
	if (!TestTrueExpr(pause_menu_widget != 0) )
		return false;

	game_user_settings = UGameUserSettings::GetGameUserSettings();
	default_GI_quality = game_user_settings->GetGlobalIlluminationQuality();
	
	auto predicate = [&container](UWidget *widget)
	{
		if (widget->GetName() == TEXT("Container") )
			container = Cast<UVerticalBox>(widget);
	};
	
	pause_menu_widget->WidgetTree->ForEachWidgetAndDescendants(predicate);
	if (!TestTrueExpr(container != 0) )
		return false;

	GI_setting = Cast<USetting_Option_Widget>(container->GetChildAt(2) );
	if (!TestTrueExpr(GI_setting != 0) )
		return false;

	right_button = Cast<UButton>(GI_setting->GetWidgetFromName(TEXT("Right_Button") ) );
	if (!TestTrueExpr(right_button != 0) )
		return false;
	
	right_button->OnClicked.Broadcast();
	if (!TestTrueExpr(game_user_settings->GetGlobalIlluminationQuality() == ( (default_GI_quality + 1) % 4) ) )
		return false;
	
	game_user_settings->SetGlobalIlluminationQuality(default_GI_quality);
	game_user_settings->ApplySettings(false);
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FRun_Benchmark
//------------------------------------------------------------------------------------------------------------
bool FRun_Benchmark::RunTest(const FString &parameters)
{
	int i;
	UWorld *world = 0;
	UPause_Menu_Widget *pause_menu_widget = 0;
	UButton *benchmark_button = 0;
	UGame_User_Settings *game_user_settings = 0;
	TArray<int> default_video_settings;
	TArray<int> first_benchmark_video_settings;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;

	game_user_settings = UGame_User_Settings::Get();
	if (!TestTrueExpr(game_user_settings != 0) )
		return false;
	
	for (TFunction<int()> &getter : game_user_settings->Video_Settings_Getters)
		default_video_settings.Add(getter() );
	
	pause_menu_widget = AsTest_Utils::Get_Widget_By_Class<UPause_Menu_Widget>(world);
	if (!TestTrueExpr(pause_menu_widget != 0) )
		return false;

	benchmark_button = Cast<UButton>(pause_menu_widget->GetWidgetFromName(TEXT("Run_Benchmark_Button") ) );
	if (!TestTrueExpr(benchmark_button != 0) )
		return false;
	benchmark_button->OnClicked.Broadcast();

	for (TFunction<int()> &getter : game_user_settings->Video_Settings_Getters)
		first_benchmark_video_settings.Add(getter() );
	
	for (i = 0; i < game_user_settings->Video_Settings_Setters.Num(); i++)
	{
		TFunction<int()> &curr_getter = game_user_settings->Video_Settings_Getters[i];
		TFunction<void(int setting_value)> &curr_setter = game_user_settings->Video_Settings_Setters[i];

		curr_setter( (curr_getter() + 1) % 4);
	}
	
	benchmark_button->OnClicked.Broadcast();

	for (i = 0; i < game_user_settings->Video_Settings_Setters.Num(); i++)
	{
		TFunction<int()> &curr_getter = game_user_settings->Video_Settings_Getters[i];
		TestTrueExpr(curr_getter() == first_benchmark_video_settings[i]);
	}

	for (i = 0; i < default_video_settings.Num(); i++)
	{
		TFunction<void(int setting_value)> &curr_setter = game_user_settings->Video_Settings_Setters[i];
		curr_setter(default_video_settings[i]);
	}

	game_user_settings->ApplySettings(false);
	
	return true;
}
//------------------------------------------------------------------------------------------------------------

