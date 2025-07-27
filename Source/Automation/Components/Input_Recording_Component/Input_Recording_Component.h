#pragma once

#include "Components/ActorComponent.h"
#include "Data/Input_Recording_Data/Input_Recording_Data.h"
#include "Input_Recording_Component.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Input_Recording_Log, All, All);

// UInput_Recording_Component
//------------------------------------------------------------------------------------------------------------
UCLASS(meta = (BlueprintSpawnableComponent) ) class UInput_Recording_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	UInput_Recording_Component();
	virtual void BeginPlay();
	virtual void TickComponent(float delta_time, ELevelTick tick_type, FActorComponentTickFunction *this_tick_func);
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason);

	UPROPERTY(EditAnywhere) FString File_Name;

private:
	FInput_Data Input_Data;
};
//------------------------------------------------------------------------------------------------------------
