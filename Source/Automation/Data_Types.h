#pragma once

#include "CoreMinimal.h"
#include "Data_Types.generated.h"

//------------------------------------------------------------------------------------------------------------
UENUM (BlueprintType) enum class EInventory_Item_Types: uint8
{
	Sphere = 0,
	Cube,
	Cylinder
};
//------------------------------------------------------------------------------------------------------------
USTRUCT (BlueprintType) struct FItem_Data
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = "0") ) EInventory_Item_Types Item_Type;
};
//------------------------------------------------------------------------------------------------------------
