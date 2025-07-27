#pragma once

class AUTOMATION_API ABattery
{
public:
	ABattery();
	ABattery(double percent);

	double Get_Percent() const;
	FColor Get_Color() const;
	FString To_String() const;
	void Charge(double percent);
	void Uncharge(double percent);

	bool operator == (const ABattery &other);
	bool operator < (const ABattery &other);

private:
	void Set_Percent(double new_percent);

	double Percent;
	FColor Color;
};
