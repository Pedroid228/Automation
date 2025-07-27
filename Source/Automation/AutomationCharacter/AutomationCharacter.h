// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Items/Inventory_Item.h"
#include "Components/Inventory_Component.h"
#include "AutomationCharacter.generated.h"

USTRUCT(BlueprintType) struct FHealth_Data
{
	GENERATED_BODY()

	FHealth_Data();
	FHealth_Data(float max_health, float heal_per_sec, float heal_rate, float life_span);
	FString To_String();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float Max_Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float Heal_Per_Sec;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(Units="s") ) float Heal_Rate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float Life_Span;
};

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UDamageType;
class AController;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AAutomationCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAutomationCharacter();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintPure) float Get_Health_Percent();


protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BlueprintProtected = "true"), Category = Health) FHealth_Data Health_Data;

private:
	UFUNCTION() void Pick_Up(UPrimitiveComponent *overlapped_comp, AActor *other_actor, UPrimitiveComponent* other_comp, int other_body_index, bool from_sweep, const FHitResult &sweep_result);
	UFUNCTION() void Receive_Damage(AActor *damaged_actor, float damage, const UDamageType *damage_type, AController *instigated_by, AActor *damage_causer);
	void On_Healing();

	float Health;
	bool Is_Dead;
	FTimerHandle Healing_Timer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true")) UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true")) UInventory_Component *Inventory_Component;
};

