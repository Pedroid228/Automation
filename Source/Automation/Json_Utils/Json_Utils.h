#pragma once

#include "Data/Input_Recording_Data/Input_Recording_Data.h"

DECLARE_LOG_CATEGORY_EXTERN(Json_Utils_Log, All, All);

// AInventory_Item
//------------------------------------------------------------------------------------------------------------
class Json_Utils
{
public:
	static bool Write_Input_Data(const FString &file_name, const FInput_Data &input_data);
	static bool Read_Input_Data(const FString &file_name, FInput_Data &input_data);
};
//------------------------------------------------------------------------------------------------------------
