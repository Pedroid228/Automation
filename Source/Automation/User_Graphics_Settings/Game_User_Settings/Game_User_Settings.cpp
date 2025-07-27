#include "Game_User_Settings.h"

// UGame_User_Settings
//------------------------------------------------------------------------------------------------------------
UGame_User_Settings::UGame_User_Settings()
{
}
//------------------------------------------------------------------------------------------------------------
UGame_User_Settings *UGame_User_Settings::Get()
{
	return Cast<UGame_User_Settings>(UGameUserSettings::GetGameUserSettings() );
}
//------------------------------------------------------------------------------------------------------------
