#include "Science_Function_Lib_Test.h"
#include "..//Science/Science_Function_Lib.h"
#include "Test_Payload.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacci_Simple, "Test_Auto.Science.Fibonacci.Simple", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacci_Stress, "Test_Auto.Science.Fibonacci.Stress", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::LowPriority | EAutomationTestFlags::StressFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST( FFibonacci_Log_Has_Errors, "Test_Auto.Science.Fibonacci.Log_Has_Errors", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )

//------------------------------------------------------------------------------------------------------------
bool FFibonacci_Simple::RunTest( const FString &Parameters )
{
	AddInfo( TEXT( "Simple test for UScience_Function_Lib::Fibonacci( int index )" ) );

	TArray<STest_Payload<int, int>> test_data{ {0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5} };

	for (auto &test : test_data)
	{
		const FString Info_String = FString::Printf(TEXT("Test value: %i, Expected value: %i"), test.Test_Value, test.Expected_Value );
		TestEqual( Info_String, UScience_Function_Lib::Fibonacci(test.Test_Value), test.Expected_Value );
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FFibonacci_Stress::RunTest( const FString &Parameters )
{
	int i;
	int current_value, prev_value, prev_prev_value;

	AddInfo( TEXT( "Stress test for UScience_Function_Lib::Fibonacci( int index )" ) );

	prev_value = 1;
	prev_prev_value = 0;

	for (i = 2; i < 40; i++)
	{
		current_value = UScience_Function_Lib::Fibonacci(i);
		const FString Info_String = FString::Printf(TEXT("Test value: %i, Test result value: %i"), i, current_value );
		TestEqual( Info_String, current_value, prev_value + prev_prev_value );

		prev_prev_value = prev_value;
		prev_value = current_value;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FFibonacci_Log_Has_Errors::RunTest( const FString &Parameters )
{
	AddInfo( TEXT( "Log has errors test for UScience_Function_Lib::Fibonacci( int index )" ) );

	AddExpectedError("Invalid negative index in Fibonacci", EAutomationExpectedErrorFlags::Contains);
	UScience_Function_Lib::Fibonacci(-10);

	return true;
}
//------------------------------------------------------------------------------------------------------------
