#include "Player_Controller.h"

DEFINE_LOG_CATEGORY(Player_Controller_Log)

// APlayer_Controller
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent == 0)
		UE_LOG(Player_Controller_Log, Error, TEXT("InputComponent was null") );

	InputComponent->BindAction(TEXT("Toggle_Game_Pause"), IE_Pressed, this, &APlayer_Controller::Toggle_Pause).bExecuteWhenPaused = true;
}
//------------------------------------------------------------------------------------------------------------
void APlayer_Controller::Toggle_Pause()
{
	bool is_paused;

	is_paused = IsPaused();

	bShowMouseCursor = !is_paused;
	SetPause(!is_paused);
}
//------------------------------------------------------------------------------------------------------------

