#pragma once

#include "InputActionValue.h"
#include "InputAction.h"
#include "Input_Recording_Data.generated.h"

//// FInput_Action_Value
////------------------------------------------------------------------------------------------------------------
//USTRUCT() struct FInput_Action_Value
//{
//	GENERATED_BODY()
//
//	FInputActionValue Input_Value;
//	FORCEINLINE FString ToString() const;
//};
////------------------------------------------------------------------------------------------------------------




// FAction_Data
//------------------------------------------------------------------------------------------------------------
USTRUCT() struct FAction_Data
{
	GENERATED_BODY()

	UPROPERTY() const UInputAction *Action = 0;
	UPROPERTY() FVector Action_Value;
};
//------------------------------------------------------------------------------------------------------------




// FBindings_Data
//------------------------------------------------------------------------------------------------------------
USTRUCT() struct FBindings_Data
{
	GENERATED_BODY()

	UPROPERTY() double Time;
	UPROPERTY() TArray<FAction_Data> Actions;
};
//------------------------------------------------------------------------------------------------------------




// FInput_Data
//------------------------------------------------------------------------------------------------------------
USTRUCT() struct FInput_Data
{
	GENERATED_BODY()

	UPROPERTY() FTransform Initial_Transform;
	UPROPERTY() TArray<FBindings_Data> Bindings;
};
//------------------------------------------------------------------------------------------------------------