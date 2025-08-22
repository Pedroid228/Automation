// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Automation_HUD.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(HUD_Log, Log, All)

class UPause_Menu_Widget;

UCLASS() class AUTOMATION_API AAutomation_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AAutomation_HUD();

	UPause_Menu_Widget *Get_Pause_Menu_Widget();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UUserWidget *Player_HUD_Widget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) UPause_Menu_Widget *Pause_Menu_Widget;

private:
	UFUNCTION() void Show_Pause_Menu(bool is_visible);
};
