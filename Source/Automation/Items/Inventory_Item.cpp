#include "Inventory_Item.h"

// AInventory_Item
//------------------------------------------------------------------------------------------------------------
AInventory_Item::AInventory_Item()
: Score(0), Item_Type(EInventory_Item_Types::Cube)
{
	// Collision_Component ROOT
	Collision_Component = CreateDefaultSubobject<USphereComponent>(TEXT("Collision_Comp") );
	Collision_Component->SetSphereRadius(30.0f);
	SetRootComponent(Collision_Component);

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main_Mesh") );
	Mesh->SetRelativeLocation(FVector(0.0, 0.0, -25.0) );
	Mesh->SetWorldScale3D(FVector(0.5, 0.5, 0.5) );
	Mesh->SetupAttachment(RootComponent);
}
//------------------------------------------------------------------------------------------------------------
