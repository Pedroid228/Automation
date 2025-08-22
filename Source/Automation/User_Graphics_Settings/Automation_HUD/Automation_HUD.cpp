#include "Automation_HUD.h"
#include "../Widgets/Pause_Menu_Widget/Pause_Menu_Widget.h"
#include "Blueprint/UserWidget.h"
#include "../Automation_Game_Mode/Automation_Game_Mode.h"

DEFINE_LOG_CATEGORY(HUD_Log)

// AAutomation_HUD
//------------------------------------------------------------------------------------------------------------
AAutomation_HUD::AAutomation_HUD()
{
}
//------------------------------------------------------------------------------------------------------------
UPause_Menu_Widget* AAutomation_HUD::Get_Pause_Menu_Widget()
{
	return Pause_Menu_Widget;
}
//------------------------------------------------------------------------------------------------------------
void AAutomation_HUD::BeginPlay()
{
	UWorld *world = 0;
	AGameModeBase* game_mode = 0;

	if (!bShowHUD)
		ShowHUD();

	if (Player_HUD_Widget == 0)
	{
		UE_LOG(HUD_Log, Error, TEXT(">>>>>>Player_HUD_Widget don't selected in BP HUD") );
		return;
	}

	if (Pause_Menu_Widget == 0)
	{
		UE_LOG(HUD_Log, Error, TEXT(">>>>>>Pause_Menu_Widget don't selected in BP HUD") );
		return;
	}

	world = GetWorld();
	if (world == 0)
	{
		UE_LOG(HUD_Log, Error, TEXT(">>>>>>World was null") );
		return;
	}

	game_mode = world->GetAuthGameMode();

	if (AAutomation_Game_Mode *auto_game_mode = Cast<AAutomation_Game_Mode>(game_mode) )
		auto_game_mode->On_Pause.AddDynamic(this, &AAutomation_HUD::Show_Pause_Menu);
	else
	{
		UE_LOG(HUD_Log, Error, TEXT(">>>>>>Invalid Game Mode") );
		return;
	}

	Player_HUD_Widget->AddToViewport();

	Pause_Menu_Widget->AddToViewport();
	Pause_Menu_Widget->SetVisibility(ESlateVisibility::Collapsed);
}
//------------------------------------------------------------------------------------------------------------
void AAutomation_HUD::Show_Pause_Menu(bool is_visible)
{
	if (is_visible)
		Pause_Menu_Widget->SetVisibility(ESlateVisibility::Visible);
	else
		Pause_Menu_Widget->SetVisibility(ESlateVisibility::Collapsed);
}
//------------------------------------------------------------------------------------------------------------
