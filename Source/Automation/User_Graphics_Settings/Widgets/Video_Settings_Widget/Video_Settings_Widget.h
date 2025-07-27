#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Setting_Option_Widget/Setting_Option_Widget.h"
#include "Video_Settings_Widget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Video_Settings_Log, Log, All)

class UVerticalBox;

// UVideo_Settings_Widget
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract) class AUTOMATION_API UVideo_Settings_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	UVideo_Settings_Widget(const FObjectInitializer &object_initializer);
	void Update();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget) ) UVerticalBox *Container = 0;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<USetting_Option_Widget> WBP_Setting_Option_Class;
private:
	void Add_Video_Setting_Widget(const FString &option_name, TFunction<void(int setting_value)> setting_value_setter, TFunction<int()> setting_value_getter);
	//static bool Is_Game_User_Setting_Valid(UGameUserSettings *game_user_settings);

	TArray<FString> Settings_Names;
};
//------------------------------------------------------------------------------------------------------------
