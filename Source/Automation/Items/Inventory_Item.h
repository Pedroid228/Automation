#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "GameFramework/Actor.h"
#include "../Data_Types.h"
#include "Inventory_Item.generated.h"

UCLASS() class AUTOMATION_API AInventory_Item : public AActor
{
	GENERATED_BODY()

public:
	AInventory_Item();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0") ) int Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EInventory_Item_Types Item_Type;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) USphereComponent *Collision_Component;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) UStaticMeshComponent *Mesh;
};
