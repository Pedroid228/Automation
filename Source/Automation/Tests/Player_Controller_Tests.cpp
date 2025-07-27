#include "Automation/Tests/Player_Controller_Tests.h"
#include "Misc/AutomationTest.h"
#include "Test_Utils.h"
#include "Components/InputComponent.h"
#include "Tests/AutomationCommon.h"

extern DEFINE_LOG_CATEGORY(Test_Log)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGame_Can_Be_Paused, "Test_Auto.Player_Controller.Game_Can_Be_Paused", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGame_Can_Be_Unpaused, "Test_Auto.Player_Controller.Game_Can_Be_Unpaused", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter)

// FGame_Can_Be_Paused
//------------------------------------------------------------------------------------------------------------
bool FGame_Can_Be_Paused::RunTest(const FString &parameters)
{
	UWorld *world = 0;
	APlayerController *PC = 0;

	ALevel_Scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;

	PC = world->GetFirstPlayerController();
	if (!TestTrueExpr(PC != 0) )
		return false;

	TestTrueExpr(!PC->IsPaused() );
	AsTest_Utils::Pause_Pressed(PC);
	TestTrueExpr(PC->IsPaused() );
	
	return true;
}
//------------------------------------------------------------------------------------------------------------




// FGame_Can_Be_Unpaused
//------------------------------------------------------------------------------------------------------------
bool FGame_Can_Be_Unpaused::RunTest(const FString &parameters)
{
	UWorld *world = 0;
	APlayerController *PC = 0;

	ALevel_Scope level_scope(TEXT("/Game/Maps/Empty_Test_Map") );
	
	world = AsTest_Utils::Get_Game_World();
	if (!TestTrueExpr(world != 0) )
		return false;

	PC = world->GetFirstPlayerController();
	if (!TestTrueExpr(PC != 0) )
		return false;

	TestTrueExpr(!PC->IsPaused() );
	AsTest_Utils::Pause_Pressed(PC);
	TestTrueExpr(PC->IsPaused() );
	AsTest_Utils::Pause_Pressed(PC);
	TestTrueExpr(!PC->IsPaused() );
	
	// auto pause = [PC]()
	// {
	// 	if (PC == 0)
	// 		UE_LOG(Test_Log, Error, TEXT(">>>>>[%hs] PC was null"), __FUNCTION__);
	// 	
	// 	if (PC->IsPaused() )
	// 		UE_LOG(Test_Log, Error, TEXT("[%hs] When try pause, game was already paused"), __FUNCTION__);
	// 	
	// 	AsTest_Utils::Pause_Pressed(PC);
	// };
	//
	// auto unpause = [PC]()
	// {
	// 	if (PC == 0)
	// 		UE_LOG(Test_Log, Error, TEXT(">>>>>[%hs] PC was null"), __FUNCTION__);
	// 	
	// 	if (! PC->IsPaused() )
	// 		UE_LOG(Test_Log, Error, TEXT("[%hs] When try unpause, game was already paused"), __FUNCTION__);
	// 	AsTest_Utils::Pause_Pressed(PC);
	// };
	//
	// ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand( pause,2.0f) );
	// ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand( unpause,2.0f) );
	// ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0) );
	//
	// TestTrueExpr(!PC->IsPaused() );
	
	return true;
}
//------------------------------------------------------------------------------------------------------------
