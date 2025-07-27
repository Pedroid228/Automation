#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Setting_Option_Widget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Setting_Option_Log, Log, All)

class UButton;
class UTextBlock;

// FSetting_Option
//------------------------------------------------------------------------------------------------------------
USTRUCT() struct AUTOMATION_API FSetting_Option
{
	GENERATED_BODY()

	FString Option_Name;
	TFunction<void(int setting_value)> Setting_Value_Setter;
	TFunction<int()> Setting_Value_Getter;
	TArray<FString> *Settings_Names;
};
//------------------------------------------------------------------------------------------------------------




// USetting_Option_Widget
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract) class AUTOMATION_API USetting_Option_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	USetting_Option_Widget(const FObjectInitializer &object_initializer);
	void Init(FSetting_Option &setting_option);
	void Update();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget) ) UButton *Left_Button = 0;
	UPROPERTY(meta = (BindWidget) ) UButton *Right_Button = 0;
	UPROPERTY(meta = (BindWidget) ) UTextBlock *Option_Name = 0;
	UPROPERTY(meta = (BindWidget) ) UTextBlock *Setting_Name = 0;

private:
	UFUNCTION() void On_Next_Setting();
	UFUNCTION() void On_Prev_Setting();

	FSetting_Option Setting_Option;
};
//------------------------------------------------------------------------------------------------------------