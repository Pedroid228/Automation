#pragma once

#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ATurret_Projectile;

// ATurret
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract) class AUTOMATION_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:
	ATurret();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Start_Projectiles_Count;
	UPROPERTY(BlueprintReadWrite) int32 Curr_Projectiles_Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) double Fire_Frequency;
	UPROPERTY(EditAnywhere) UStaticMeshComponent *Turret_Mesh = 0;
	UPROPERTY(EditAnywhere) TSubclassOf<ATurret_Projectile> Turret_Projectile_BP;

private:
	UFUNCTION() void On_Fire();
	FTimerHandle Fire_Timer;
};
//------------------------------------------------------------------------------------------------------------
