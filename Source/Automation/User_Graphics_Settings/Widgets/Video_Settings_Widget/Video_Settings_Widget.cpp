#include "Video_Settings_Widget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Widget.h"
#include "User_Graphics_Settings/Game_User_Settings/Game_User_Settings.h"

DEFINE_LOG_CATEGORY(Video_Settings_Log)

// UVideo_Settings_Widget
//------------------------------------------------------------------------------------------------------------
UVideo_Settings_Widget::UVideo_Settings_Widget(const FObjectInitializer& object_initializer)
: Super(object_initializer)
{
	Settings_Names = {TEXT("Low"), TEXT("Medium"), TEXT("High"), TEXT("Epic") };
}
//------------------------------------------------------------------------------------------------------------
void UVideo_Settings_Widget::Update()
{
	TArray<UWidget *> children;

	children = Container->GetAllChildren();

	for (UWidget *child : children)
	{
		if (child == 0)
		{
			UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Child was null"), __FUNCTION__);
			return;
		}

		if (USetting_Option_Widget *setting_option_widget = Cast<USetting_Option_Widget>(child) )
		{
			setting_option_widget->Update();
		}
		else
		{
			UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Child was not casted"), __FUNCTION__);
			return;
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void UVideo_Settings_Widget::NativeConstruct()
{
	FSetting_Option setting_option;
	USetting_Option_Widget *option_widget = 0;
	UGameUserSettings *game_user_settings;
	TFunction<void(int setting_value)> setting_value_setter;
	TFunction<int()> setting_value_getter;

	Super::NativeConstruct();

	if (Container == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Container was null"), __FUNCTION__);
		return;
	}

	Container->ClearChildren();

	if (WBP_Setting_Option_Class == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] WBP_Setting_Option_Class was not set"), __FUNCTION__);
		return;
	}

	option_widget = CreateWidget<USetting_Option_Widget>(this, WBP_Setting_Option_Class);
	if (option_widget == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Option Widget was null"), __FUNCTION__);
		return;
	}

	game_user_settings = UGameUserSettings::GetGameUserSettings();
	if (game_user_settings == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] game_user_settings was null"), __FUNCTION__);
		return;
	}

	game_user_settings->LoadSettings(); // Load User Settings

	// Anti-Aliasing
	setting_value_setter = [game_user_settings](int setting_value)
	{
		game_user_settings->SetAntiAliasingQuality(setting_value);
	};

	setting_value_getter = [game_user_settings]()
	{
		return game_user_settings->GetAntiAliasingQuality();
	};

	Add_Video_Setting_Widget(TEXT("Anti-Aliasing"), setting_value_setter, setting_value_getter);

	// Textures
	setting_value_setter = [game_user_settings](int setting_value)
	{
		game_user_settings->SetTextureQuality(setting_value);
	};

	setting_value_getter = [game_user_settings]()
	{
		return game_user_settings->GetTextureQuality();
	};

	Add_Video_Setting_Widget(TEXT("Textures"), setting_value_setter, setting_value_getter);

	// Global Illumination
	setting_value_setter = [game_user_settings](int setting_value)
	{
		game_user_settings->SetGlobalIlluminationQuality(setting_value);
	};

	setting_value_getter = [game_user_settings]()
	{
		return game_user_settings->GetGlobalIlluminationQuality();
	};

	Add_Video_Setting_Widget(TEXT("Global Illumination"), setting_value_setter, setting_value_getter);

	// Shadows
	setting_value_setter = [game_user_settings](int setting_value)
	{
		game_user_settings->SetShadowQuality(setting_value);
	};

	setting_value_getter = [game_user_settings]()
	{
		return game_user_settings->GetShadowQuality();
	};

	Add_Video_Setting_Widget(TEXT("Shadows"), setting_value_setter, setting_value_getter);

	// Post Processing
	setting_value_setter = [game_user_settings](int setting_value)
		{
			game_user_settings->SetPostProcessingQuality(setting_value);
		};

	setting_value_getter = [game_user_settings]()
		{
			return game_user_settings->GetPostProcessingQuality();
		};

	Add_Video_Setting_Widget(TEXT("Post Processing"), setting_value_setter, setting_value_getter);

	// Reflections
	setting_value_setter = [game_user_settings](int setting_value)
		{
			game_user_settings->SetReflectionQuality(setting_value);
		};

	setting_value_getter = [game_user_settings]()
		{
			return game_user_settings->GetReflectionQuality();
		};

	Add_Video_Setting_Widget(TEXT("Reflections"), setting_value_setter, setting_value_getter);

	// Reflections
	setting_value_setter = [game_user_settings](int setting_value)
		{
			game_user_settings->SetVisualEffectQuality(setting_value);
		};

	setting_value_getter = [game_user_settings]()
		{
			return game_user_settings->GetVisualEffectQuality();
		};

	Add_Video_Setting_Widget(TEXT("Effects"), setting_value_setter, setting_value_getter);
}
//------------------------------------------------------------------------------------------------------------
void UVideo_Settings_Widget::Add_Video_Setting_Widget(const FString &option_name, TFunction<void(int setting_value)> setting_value_setter, TFunction<int()> setting_value_getter)
{
	FSetting_Option setting_option;
	USetting_Option_Widget *option_widget = 0;
	UGame_User_Settings *game_user_settings = 0;

	game_user_settings = UGame_User_Settings::Get();
	if (game_user_settings == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] game_user_settings was null"), __FUNCTION__);
		return;
	}

	game_user_settings->Video_Settings_Getters.Add(setting_value_getter);
	game_user_settings->Video_Settings_Setters.Add(setting_value_setter);
	
	option_widget = CreateWidget<USetting_Option_Widget>(this, WBP_Setting_Option_Class);
	if (option_widget == 0)
	{
		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Option Widget was null"), __FUNCTION__);
		return;
	}

	// Setup option
	setting_option.Option_Name = option_name;
	setting_option.Settings_Names = &Settings_Names;
	setting_option.Setting_Value_Setter = setting_value_setter;
	setting_option.Setting_Value_Getter = setting_value_getter;

	option_widget->Init(setting_option);
	Container->AddChildToVerticalBox(option_widget);
}
//------------------------------------------------------------------------------------------------------------
// bool UVideo_Settings_Widget::Is_Game_User_Setting_Valid(TSharedObjectPtr<UGameUserSettings> game_user_settings)
// {
// 	if (!game_user_settings.IsValid() )
// 	{
// 		UE_LOG(Video_Settings_Log, Error, TEXT(">>>>>>[%hs] Invalid pointer Game User Settings"), __FUNCTION__);
// 		return false;
// 	}
//
// 	return true;
// }
// //------------------------------------------------------------------------------------------------------------
