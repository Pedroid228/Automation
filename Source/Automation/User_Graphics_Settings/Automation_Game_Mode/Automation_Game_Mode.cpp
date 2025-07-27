#include "Automation_Game_Mode.h"
#include "UObject/ConstructorHelpers.h"

// AAutomation_Game_Mode
//------------------------------------------------------------------------------------------------------------
AAutomation_Game_Mode::AAutomation_Game_Mode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
//------------------------------------------------------------------------------------------------------------
bool AAutomation_Game_Mode::SetPause(APlayerController* PC, FCanUnpause can_unpause_delegate)
{
	On_Pause.Broadcast(true);

	return Super::SetPause(PC, can_unpause_delegate);
}
//------------------------------------------------------------------------------------------------------------
bool AAutomation_Game_Mode::ClearPause()
{
	On_Pause.Broadcast(false);

	return Super::ClearPause();
}
//------------------------------------------------------------------------------------------------------------
