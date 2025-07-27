#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Science_Function_Lib.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS() class AUTOMATION_API UScience_Function_Lib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure) static int Fibonacci(int index);
};
//------------------------------------------------------------------------------------------------------------
