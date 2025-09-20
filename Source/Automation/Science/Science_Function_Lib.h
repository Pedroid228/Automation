#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Science_Function_Lib.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS() class AUTOMATION_API UScience_Function_Lib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/*! \f$F(n)=(F(n - 1))+(F(n - 2))\f$ */
	UFUNCTION(BlueprintPure) static int Fibonacci(int index);
	/*! \f$n! = (n)(n - 1)...(2)(1)\f$ */
	UFUNCTION(BlueprintPure) static int Factorial(int index);
};
//------------------------------------------------------------------------------------------------------------
