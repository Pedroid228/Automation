#include "Turret_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"

// ATurret_Projectile
//------------------------------------------------------------------------------------------------------------
ATurret_Projectile::ATurret_Projectile()
: Damage_Amount(15.0f), Life_Time(5.0f)
{
	Sphere_Component = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere_Collision") );
	check(Sphere_Component != 0);
	Sphere_Component->SetSphereRadius(50.0f);
	Sphere_Component->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere_Component->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(Sphere_Component);

	Projectile_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile_Mesh") );
	check(Projectile_Mesh != 0);
	Projectile_Mesh->SetupAttachment(GetRootComponent() );
	
	Movement_Component = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement_Component") );
	check(Movement_Component != 0);
	Movement_Component->InitialSpeed = 100.0f;
	Movement_Component->MaxSpeed = 100.0f;
	Movement_Component->ProjectileGravityScale = 0.0f;

	PrimaryActorTick.bCanEverTick = false;
}
//------------------------------------------------------------------------------------------------------------
void ATurret_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Life_Time);
	Movement_Component->Velocity = Movement_Direction.GetSafeNormal() * Movement_Component->InitialSpeed;
	Sphere_Component->OnComponentHit.AddDynamic(this, &ATurret_Projectile::On_Hit);
}
//------------------------------------------------------------------------------------------------------------
void ATurret_Projectile::On_Hit(UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent* other_comp, FVector normal_impulse, const FHitResult &hit)
{
	if (other_actor != 0)
		other_actor->TakeDamage(Damage_Amount, FDamageEvent{}, 0, this);

	Movement_Component->StopMovementImmediately();
	Destroy();
}
//------------------------------------------------------------------------------------------------------------

