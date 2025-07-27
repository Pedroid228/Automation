// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Delegates/DelegateCombinations.h"
#include "Automation_Game_Mode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn_Pause, bool, is_paused);

// AAutomation_Game_Mode
//------------------------------------------------------------------------------------------------------------
UCLASS(minimalapi) class AAutomation_Game_Mode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAutomation_Game_Mode();
	FOn_Pause On_Pause;

protected:
	virtual bool SetPause(APlayerController* PC, FCanUnpause can_unpause_delegate = FCanUnpause()) override;
	virtual bool ClearPause() override;
};
//------------------------------------------------------------------------------------------------------------


