#pragma once

#include "GameFramework/Actor.h"
#include "Turret_Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

// ATurret_Projectile
//------------------------------------------------------------------------------------------------------------
UCLASS(Abstract) class AUTOMATION_API ATurret_Projectile: public AActor
{
	GENERATED_BODY()
	
public:	
	ATurret_Projectile();
	FVector Movement_Direction;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere) float Damage_Amount; /*!< Damage from this projectile */
	UPROPERTY(EditAnywhere, Meta=(Units="Seconds") ) float Life_Time; /*!< Projectile life time */
	UPROPERTY(EditAnywhere) UProjectileMovementComponent *Movement_Component = 0;
	UPROPERTY(EditAnywhere) UStaticMeshComponent *Projectile_Mesh = 0;
	UPROPERTY(EditAnywhere) USphereComponent *Sphere_Component = 0;
	
private:
	UFUNCTION() void On_Hit(UPrimitiveComponent *hit_component, AActor* other_actor, UPrimitiveComponent* other_comp, FVector normal_impulse, const FHitResult &hit);
};
//------------------------------------------------------------------------------------------------------------
