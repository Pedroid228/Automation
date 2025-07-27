#include "Setting_Option_Widget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"

DEFINE_LOG_CATEGORY(Setting_Option_Log)

// USetting_Option_Widget
//------------------------------------------------------------------------------------------------------------
USetting_Option_Widget::USetting_Option_Widget(const FObjectInitializer &object_initializer)
: Super(object_initializer)
{
}
//------------------------------------------------------------------------------------------------------------
void USetting_Option_Widget::Init(FSetting_Option &setting_option)
{
	int setting_value;

	Setting_Option = setting_option;

	Option_Name->SetText(FText::FromString(Setting_Option.Option_Name) );  // Option Name
	
	// Update Widget
	setting_value = Setting_Option.Setting_Value_Getter();
	Setting_Option.Setting_Value_Setter(setting_value);
	Update();
}
//------------------------------------------------------------------------------------------------------------
void USetting_Option_Widget::Update()
{
	int curr_setting_value;
	FString new_setting_name;
	UGameUserSettings *game_user_settings = 0;

	if (Setting_Option.Settings_Names == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Settings Names was null"), __FUNCTION__);
		return;
	}

	curr_setting_value = Setting_Option.Setting_Value_Getter();

	if (! Setting_Option.Settings_Names->IsValidIndex(curr_setting_value) )
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Invalid setting name index"), __FUNCTION__);
		return;
	}
	new_setting_name = (*Setting_Option.Settings_Names)[curr_setting_value];
	Setting_Name->SetText(FText::FromString(new_setting_name) );

	game_user_settings = UGameUserSettings::GetGameUserSettings();
	if (game_user_settings == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Game User Settings was null"), __FUNCTION__);
		return;
	}

	game_user_settings->ApplySettings(true);
}
//------------------------------------------------------------------------------------------------------------
void USetting_Option_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Left_Button == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Left_Button was null"), __FUNCTION__);
		return;
	}

	if (Right_Button == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Left_Button was null"), __FUNCTION__);
		return;
	}

	Left_Button->OnClicked.AddDynamic(this, &USetting_Option_Widget::On_Prev_Setting);
	Right_Button->OnClicked.AddDynamic(this, &USetting_Option_Widget::On_Next_Setting);
}
//------------------------------------------------------------------------------------------------------------
void USetting_Option_Widget::On_Next_Setting()
{
	int curr_setting_value;
	int new_setting_value;

	if (Setting_Option.Settings_Names == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Settings Names was null"), __FUNCTION__);
		return;
	}

	curr_setting_value = Setting_Option.Setting_Value_Getter();
	new_setting_value = (curr_setting_value + 1) % Setting_Option.Settings_Names->Num();

	Setting_Option.Setting_Value_Setter(new_setting_value);
	Update();
}
//------------------------------------------------------------------------------------------------------------
void USetting_Option_Widget::On_Prev_Setting()
{
	int curr_setting_value;
	int new_setting_value;

	if (Setting_Option.Settings_Names == 0)
	{
		UE_LOG(Setting_Option_Log, Error, TEXT(">>>>>>[%hs] Settings Names was null"), __FUNCTION__);
		return;
	}

	curr_setting_value = Setting_Option.Setting_Value_Getter();
	new_setting_value = (curr_setting_value - 1);

	if (new_setting_value < 0)
		new_setting_value = Setting_Option.Settings_Names->Num() - 1;

	Setting_Option.Setting_Value_Setter(new_setting_value);
	Update();
}
//------------------------------------------------------------------------------------------------------------
