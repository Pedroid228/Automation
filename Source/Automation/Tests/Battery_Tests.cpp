#include "../Tests/Battery_Tests.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AutomationTest.h" 
#include "Battery/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST( FTest_Class_Name, "Test_Auto.ABattery", EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::HighPriority | EAutomationTestFlags::ProductFilter )

//------------------------------------------------------------------------------------------------------------
bool Test_Custom_Constructor(double percent, const FColor &color, const FString percent_string);
//------------------------------------------------------------------------------------------------------------
bool FTest_Class_Name::RunTest( const FString &parameters )
{
	AddInfo(TEXT("Test ABattery Default Constructor") );
	ABattery battery_default_construct;
	TestTrueExpr(FMath::IsNearlyEqual(battery_default_construct.Get_Percent(), 1.0) );
	TestTrueExpr(battery_default_construct.Get_Color() == FColor::Green);
	TestTrueExpr(battery_default_construct.To_String().Equals(TEXT("100%") ) );

	AddInfo(TEXT("Test ABattery Custom Constructor") );
	TestTrueExpr(Test_Custom_Constructor(0.75, FColor::Green, TEXT("75%") ) );
	TestTrueExpr(Test_Custom_Constructor(0.53, FColor::Yellow, TEXT("53%") ) );
	TestTrueExpr(Test_Custom_Constructor(0.17, FColor::Red, TEXT("17%") ) );
	TestTrueExpr(Test_Custom_Constructor(0.0, FColor::Red, TEXT("0%") ) );
	TestTrueExpr(Test_Custom_Constructor(1.0, FColor::Green, TEXT("100%") ) );

	AddInfo(TEXT("Test ABattery Charge / Uncharge") );
	ABattery battery_object(0.7);

	battery_object.Uncharge(0.2);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 0.5) );
	battery_object.Uncharge(0.1);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 0.4) );
	battery_object.Charge(0.3);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 0.7) );
	battery_object.Charge(0.3);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 1.0) );

	AddInfo(TEXT("Corner Test ABattery Charge / Uncharge") );

	int i;
	for (i = 0; i < 14; i++)
		battery_object.Uncharge(0.1);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 0.0) );

	for (i = 0; i < 14; i++)
		battery_object.Charge(0.1);
	TestTrueExpr(FMath::IsNearlyEqual(battery_object.Get_Percent(), 1.0) );

	AddInfo(TEXT("Test ABattery comparison") );
	ABattery battery_high(0.9);
	ABattery battery_low(0.2);

	TestTrueExpr(battery_low < battery_high);
	//TestTrueExpr(FMath::Max(battery_low, battery_high) == battery_high);

	AddInfo(TEXT("Test ABattery in memory comprassion") );
	ABattery battery_origin(0.2);
	ABattery battery_origin_1(0.2);

	ABattery *battery_origin_pointer = &battery_origin;
	ABattery &battery_origin_1_reference = battery_origin_1;

	TestNotSame(TEXT("Not the same batteries"), battery_origin, battery_origin_1);
	TestSame(TEXT("The same batteries"), battery_origin, *battery_origin_pointer);
	TestSame(TEXT("The same batteries"), battery_origin_1, battery_origin_1_reference);

	return true;
}
//------------------------------------------------------------------------------------------------------------
bool Test_Custom_Constructor(double percent, const FColor &color, const FString percent_string)
{
	bool percent_check, color_check, string_check;
	ABattery battery(percent);

	percent_check = FMath::IsNearlyEqual(battery.Get_Percent(), percent);
	color_check = battery.Get_Color() == color;
	string_check = battery.To_String().Equals(percent_string);

	return percent_check && color_check && string_check;
}
//------------------------------------------------------------------------------------------------------------