#include "Science_Function_Lib_Test.h"
#include "..//Science/Science_Function_Lib.h"
#include "Test_Payload.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacci_Simple, "Test_Auto.Science.Fibonacci.Simple", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacci_Stress, "Test_Auto.Science.Fibonacci.Stress", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::LowPriority | EAutomationTestFlags::StressFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacci_Log_Has_Errors, "Test_Auto.Science.Fibonacci.Log_Has_Errors", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )
DEFINE_SPEC(FFactorial_Spec, "Test_Auto.Science.Factorial_Spec", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )


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




//------------------------------------------------------------------------------------------------------------
void FFactorial_Spec::Define()
{
	Describe(TEXT("Corner cases"),  [this]()
	{
		It(TEXT("should return 1 when index is 0"), [this](){ TestTrueExpr(UScience_Function_Lib::Factorial(0) == 1); });
		It(TEXT("should return 1 when index is 1"), [this](){ TestTrueExpr(UScience_Function_Lib::Factorial(1) == 1); });
	});

	Describe(TEXT("Normal cases"),  [this]()
	{
		FString it_expectation;
		TArray<STest_Payload<int, int>> test_data{{2, 2}, {3, 6}, {4, 24}, {5, 120} };
		
		for (STest_Payload<int, int> &data : test_data)
		{
			it_expectation = FString::Printf(TEXT("should return %i when index is %i"), data.Expected_Value, data.Test_Value);

			It(it_expectation, [this, data](){ TestTrueExpr(UScience_Function_Lib::Factorial(data.Test_Value) == data.Expected_Value); });
		}
	});
}
//------------------------------------------------------------------------------------------------------------
