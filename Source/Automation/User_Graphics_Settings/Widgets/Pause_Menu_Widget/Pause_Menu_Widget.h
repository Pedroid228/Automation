#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Pause_Menu_Widget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Pause_Menu_Log, Log, All)

class UButton;
class UVideo_Settings_Widget;

// UPause_Menu_Widget
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract) class AUTOMATION_API UPause_Menu_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPause_Menu_Widget(const FObjectInitializer &object_initializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget) ) UButton *Close_Button = 0;
	UPROPERTY(meta = (BindWidget) ) UButton *Run_Benchmark_Button = 0;
	UPROPERTY(meta = (BindWidget) ) UVideo_Settings_Widget *Video_Settings = 0;

private:
	UFUNCTION() void On_Close_Button_Clicked();
	UFUNCTION() void On_Run_Benchmark_Button_Clicked();
};
//------------------------------------------------------------------------------------------------------------
