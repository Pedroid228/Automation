#pragma once

#include "GameFramework/GameUserSettings.h"
#include "Game_User_Settings.generated.h"

// UGame_User_Settings
//------------------------------------------------------------------------------------------------------------
UCLASS() class AUTOMATION_API UGame_User_Settings: public UGameUserSettings
{
	GENERATED_BODY()

public:
	UGame_User_Settings();
	static UGame_User_Settings *Get();
	
	TArray<TFunction<void(int setting_value)>> Video_Settings_Setters;
	TArray<TFunction<int()>> Video_Settings_Getters;
};
//------------------------------------------------------------------------------------------------------------
