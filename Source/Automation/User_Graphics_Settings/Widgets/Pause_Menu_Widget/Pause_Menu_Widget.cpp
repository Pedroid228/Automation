#include "Pause_Menu_Widget.h"
#include "Components/Button.h"
#include "User_Graphics_Settings/Player_Controller/Player_Controller.h"
#include "Delegates/Delegate.h"
#include "GameFramework/GameUserSettings.h"
#include "../Video_Settings_Widget/Video_Settings_Widget.h"

DEFINE_LOG_CATEGORY(Pause_Menu_Log)

// UPause_Menu_Widget
//------------------------------------------------------------------------------------------------------------
UPause_Menu_Widget::UPause_Menu_Widget(const FObjectInitializer &object_initializer)
: Super(object_initializer)
{
}
//------------------------------------------------------------------------------------------------------------
void UPause_Menu_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Close_Button == 0)
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Close_Button was null"), __FUNCTION__);

	if (Run_Benchmark_Button == 0)
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Run_Benchmark_Button was null"), __FUNCTION__);

	if (Video_Settings == 0)
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Video_Settings was null"), __FUNCTION__);

	Close_Button->OnClicked.AddDynamic(this, &UPause_Menu_Widget::On_Close_Button_Clicked);
	Run_Benchmark_Button->OnClicked.AddDynamic(this, &UPause_Menu_Widget::On_Run_Benchmark_Button_Clicked);
}
//------------------------------------------------------------------------------------------------------------
void UPause_Menu_Widget::On_Close_Button_Clicked()
{
	UWorld *world = 0;
	APlayerController *PC = 0;

	world = GetWorld();
	if (world == 0)
	{
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] World was null"), __FUNCTION__);
		return;
	}

	PC = world->GetFirstPlayerController();
	if (world == 0)
	{
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Player Controller was null"), __FUNCTION__);
		return;
	}

	if (APlayer_Controller *player_controller = Cast<APlayer_Controller>(PC) )
	{
		player_controller->Toggle_Pause();
	}
	else
	{
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Player Controller was not casted"), __FUNCTION__);
		return;
	}
}
//------------------------------------------------------------------------------------------------------------
void UPause_Menu_Widget::On_Run_Benchmark_Button_Clicked()
{
	UGameUserSettings *game_user_settings;

	game_user_settings = UGameUserSettings::GetGameUserSettings();

	game_user_settings->RunHardwareBenchmark();

	if (Video_Settings == 0)
		UE_LOG(Pause_Menu_Log, Error, TEXT(">>>>>>[%hs] Video_Settings was null"), __FUNCTION__);
	Video_Settings->Update();
}
//------------------------------------------------------------------------------------------------------------
