// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player_Controller.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Player_Controller_Log, Log, All);

// APlayer_Controller
//------------------------------------------------------------------------------------------------------------
UCLASS() class AUTOMATION_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION() void Toggle_Pause();

protected:
	virtual void SetupInputComponent() override;
};
//------------------------------------------------------------------------------------------------------------
