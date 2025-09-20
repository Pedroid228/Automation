#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Items/Inventory_Item.h"
#include "Inventory_Component.generated.h"

// AInventory
//------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) class UInventory_Component: public UActorComponent
{
	GENERATED_BODY()

public:
	UInventory_Component();
	/*! \attention if we cant add item, then nothing happens */
	bool Try_To_Add_Item(AInventory_Item *new_item);
	
	/*! \todo Get_Score_By_Type is not created */
	UFUNCTION(BlueprintCallable) int Get_Score_By_Type(EInventory_Item_Types item_type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<EInventory_Item_Types, int> Score_Limits;
protected:
	virtual void BeginPlay();

private:
	TMap<EInventory_Item_Types, int> Inventory;
};
//------------------------------------------------------------------------------------------------------------