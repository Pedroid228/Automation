#include "Science_Function_Lib.h"

DEFINE_LOG_CATEGORY_STATIC(LogFibonacci, All, All)

//------------------------------------------------------------------------------------------------------------
int UScience_Function_Lib::Fibonacci( int index )
{
	if (index < 0)
	{
		UE_LOG(LogFibonacci, Error, TEXT("Invalid negative index in Fibonacci: %i"), index);
		return -1;
	}
	else
	{
		if ( index  == 0)
			return 0;
		else
			if (index == 1)
				return 1;

		return Fibonacci( index - 1 ) + Fibonacci( index - 2);
	}
}
//------------------------------------------------------------------------------------------------------------
