#include "Battery.h"

//------------------------------------------------------------------------------------------------------------
ABattery::ABattery()
: Percent(1.0)
{
}
//------------------------------------------------------------------------------------------------------------
ABattery::ABattery(double percent)
{
	Set_Percent(percent);
}
//------------------------------------------------------------------------------------------------------------
double ABattery::Get_Percent() const
{
	return Percent;
}
//------------------------------------------------------------------------------------------------------------
FColor ABattery::Get_Color() const
{
	if (Percent >= 0.7)
		return FColor::Green;
	else
		if (Percent >= 0.4)
			return FColor::Yellow;
		else
			return FColor::Red;
}
//------------------------------------------------------------------------------------------------------------
FString ABattery::To_String() const
{
	int charge_percent = (int)(Percent * 100.0);
	FString percent_str = FString::Printf(TEXT("%d%%"), charge_percent);
	return percent_str;
}
//------------------------------------------------------------------------------------------------------------
void ABattery::Charge(double percent)
{
	double new_percent = Percent;
	new_percent += percent;
	Set_Percent(new_percent);
}
//------------------------------------------------------------------------------------------------------------
void ABattery::Uncharge(double percent)
{
	double new_percent = Percent;
	new_percent -= percent;
	Set_Percent(new_percent);
}
//------------------------------------------------------------------------------------------------------------
bool ABattery::operator == (const ABattery &other)
{
	return Percent == other.Get_Percent() ;
}
//------------------------------------------------------------------------------------------------------------
bool ABattery::operator < (const ABattery &other)
{
	return Percent < other.Get_Percent();
}
//------------------------------------------------------------------------------------------------------------
void ABattery::Set_Percent(double new_percent)
{
	Percent = FMath::Clamp(new_percent, 0.0, 1.0);
}
//------------------------------------------------------------------------------------------------------------
