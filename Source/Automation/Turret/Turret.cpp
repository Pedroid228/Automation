#include "Turret.h"
#include "../Turret_Projectile/Turret_Projectile.h"
#include "Engine/StaticMeshSocket.h"

//------------------------------------------------------------------------------------------------------------
ATurret::ATurret()
: Start_Projectiles_Count(10), Fire_Frequency(0.5), Curr_Projectiles_Count(0)
{
	Turret_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret_Mesh");
	check(Turret_Mesh != 0);
	SetRootComponent(Turret_Mesh);

	PrimaryActorTick.bCanEverTick = false;
}
//------------------------------------------------------------------------------------------------------------
void ATurret::BeginPlay()
{
	UWorld *world = 0;
	float first_delay;
	
	Super::BeginPlay();

	check(Turret_Projectile_BP != 0);
	
	world = GetWorld();
	check(world != 0);

	first_delay = Fire_Frequency;
	world->GetTimerManager().SetTimer(Fire_Timer, this, &ATurret::On_Fire, Fire_Frequency, true, first_delay);

	Curr_Projectiles_Count = Start_Projectiles_Count;
}
//------------------------------------------------------------------------------------------------------------
void ATurret::On_Fire()
{
	FVector player_pos;
	FTransform muzzle_socket_transform;
	AActor *player_actor = 0;
	UWorld *world = 0;
	ATurret_Projectile *new_projectile = 0;

	check(Curr_Projectiles_Count > 0);
	
	world = GetWorld();
	check(world != 0);

	muzzle_socket_transform = Turret_Mesh->GetSocketTransform(TEXT("Muzzle_Socket") );
	
	player_actor = world->GetFirstPlayerController()->GetPawnOrSpectator();
	check(player_actor != 0);
	player_pos = player_actor->GetActorLocation();
	
	new_projectile = world->SpawnActorDeferred<ATurret_Projectile>(Turret_Projectile_BP, muzzle_socket_transform);
	check(new_projectile != 0);
	new_projectile->Movement_Direction = player_pos - muzzle_socket_transform.GetLocation();
	new_projectile->FinishSpawning(muzzle_socket_transform);

	Curr_Projectiles_Count--;
	
	if (Curr_Projectiles_Count <= 0)
		world->GetTimerManager().ClearTimer(Fire_Timer);
}
//------------------------------------------------------------------------------------------------------------
