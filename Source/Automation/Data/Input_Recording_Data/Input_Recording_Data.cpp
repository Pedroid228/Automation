#include "Input_Recording_Data.h"

//// FInput_Action_Value
////------------------------------------------------------------------------------------------------------------
//FORCEINLINE FString FInput_Action_Value::ToString() const
//{
//	bool is_value;
//	float value_f;
//	FVector2D value_2d;
//	FVector value_3d;
//
//	switch (Input_Value.GetValueType() )
//	{
//	case EInputActionValueType::Boolean:
//		is_value = Input_Value.Get<bool>();
//		return FString(is_value ? TEXT("true") : TEXT("false"));
//
//	case EInputActionValueType::Axis1D:
//		value_f = Input_Value.Get<FInputActionValue::Axis1D>();
//		return FString::Printf(TEXT("%3.3f"), value_f);
//
//	case EInputActionValueType::Axis2D:
//		value_2d = Input_Value.Get<FInputActionValue::Axis2D>();
//		return FString::Printf(TEXT("X=%3.3f, Y=%3.3f"), value_2d.X, value_2d.Y);
//
//	case EInputActionValueType::Axis3D:
//		value_3d = Input_Value.Get<FInputActionValue::Axis3D>();
//		return FString::Printf(TEXT("X=%3.3f, Y=%3.3f, Z=%3.3f"), value_3d.X, value_3d.Y, value_3d.Z);
//	}
//
//	checkf(false, TEXT("Unsupported value type for input action value!") );
//	return FString{};
//}
////------------------------------------------------------------------------------------------------------------
