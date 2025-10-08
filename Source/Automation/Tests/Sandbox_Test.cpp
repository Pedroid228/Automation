#include "Sandbox_Test.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AutomationTest.h"
#include "..//Tests/Test_Payload.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FMath_Abs, "Test_Auto.Math.Abs", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::EngineFilter )
IMPLEMENT_SIMPLE_AUTOMATION_TEST( FMath_Sin, "Test_Auto.Math.Sin", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::EngineFilter )

//------------------------------------------------------------------------------------------------------------
bool FMath_Abs::RunTest( const FString &Parameters )
{
	AddInfo(TEXT("Test for FMath::Abs") );

	TArray<STest_Payload<int, int>> test_data_int { {5, 5}, {-5, 5} };
	TArray<STest_Payload<double, double>> test_data_double { {5.123, 5.123}, {-5.123, 5.123} };

	for (STest_Payload<int, int> &test : test_data_int)
	{
		const FString Info_String = FString::Printf(TEXT("Test value: %i, Expected value: %i"), test.Test_Value, test.Expected_Value );
		TestEqual( Info_String, FMath::Abs(test.Test_Value), test.Expected_Value );
	}

	for (STest_Payload<double, double> &test : test_data_double)
	{
		const FString Info_String = FString::Printf(TEXT("Test value: %f, Expected value: %f"), test.Test_Value, test.Expected_Value );
		TestEqual( Info_String, FMath::Abs(test.Test_Value), test.Expected_Value, test.Tolerance);
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool FMath_Sin::RunTest( const FString &Parameters )
{
	AddInfo( TEXT( "Test for FMath::Sin" ) );

	TArray<STest_Payload<double, double>> test_data { {0.0, 0.0}, {180.0, 0.0},  {45.0, 0.7071, 0.0001}, {50.3, 0.76939, 0.00001}, {200.0, -0.342}, {-150.3, -0.495} };

	for (STest_Payload<double, double> &test : test_data)
	{
		const FString Info_String = FString::Printf(TEXT("Test value: %f, Expected value: %f"), test.Test_Value, test.Expected_Value);
		double radians_value = FMath::DegreesToRadians(test.Test_Value);
		TestEqual(Info_String, FMath::Sin(radians_value), test.Expected_Value, test.Tolerance);
	}

	return true;
}
//------------------------------------------------------------------------------------------------------------
