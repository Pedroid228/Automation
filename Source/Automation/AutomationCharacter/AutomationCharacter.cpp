// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// FHealth_Data
//------------------------------------------------------------------------------------------------------------
FHealth_Data::FHealth_Data()
: Max_Health(0.0f), Heal_Per_Sec(0.0f), Heal_Rate(0.0f), Life_Span(0.0f)
{
}
//------------------------------------------------------------------------------------------------------------
FHealth_Data::FHealth_Data(float max_health, float heal_per_sec, float heal_rate, float life_span)
: Max_Health(max_health), Heal_Per_Sec(heal_per_sec), Heal_Rate(heal_rate), Life_Span(life_span)
{
}
//------------------------------------------------------------------------------------------------------------
FString FHealth_Data::To_String()
{
	return FString::Printf(TEXT("(Max_Health=%f,Heal_Per_Sec=%f,Heal_Rate=%f,Life_Span=%f)"), Max_Health, Heal_Per_Sec, Heal_Rate, Life_Span);
}
//------------------------------------------------------------------------------------------------------------




// AAutomationCharacter
//------------------------------------------------------------------------------------------------------------
AAutomationCharacter::AAutomationCharacter()
: Health_Data(100.0f, 5.0f, 0.1f, 3.0f), Health(0.0f), Is_Dead(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAutomationCharacter::Pick_Up);

		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create inventory
	Inventory_Component = CreateDefaultSubobject<UInventory_Component>(TEXT("Inventory") );

	OnTakeAnyDamage.AddDynamic(this, &AAutomationCharacter::Receive_Damage);
}
//------------------------------------------------------------------------------------------------------------
float AAutomationCharacter::Get_Health_Percent()
{
	return Health / Health_Data.Max_Health;
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAutomationCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAutomationCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = Health_Data.Max_Health;
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::Pick_Up(UPrimitiveComponent *overlapped_comp, AActor *other_actor, UPrimitiveComponent* other_comp, int other_body_index, bool from_sweep, const FHitResult &sweep_result)
{
	if (AInventory_Item *new_item = Cast<AInventory_Item>(other_actor) )
	{
		if (Inventory_Component != 0)
		{
			FItem_Data data;
			data.Item_Type = new_item->Item_Type;
			data.Score = new_item->Score;

			if (Inventory_Component->Try_To_Add_Item(new_item) )
				new_item->Destroy();
		}
	}
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::Receive_Damage(AActor *damaged_actor, float damage, const UDamageType *damage_type, AController *instigated_by, AActor *damage_causer)
{
	USkeletalMeshComponent *mesh = 0;
	UCapsuleComponent *capsule = 0;
	UMovementComponent *movement_component;

	if (damage <= 0.0f || Is_Dead)
		return;

	Health = FMath::Clamp(Health - damage, 0.0f, Health_Data.Max_Health);

	if (Health <= 0.0f)
	{
		mesh = GetMesh();
		if (mesh == 0)
			throw 13;

		capsule = GetCapsuleComponent();
		if (capsule == 0)
			throw 13;

		movement_component = GetMovementComponent();
		if (movement_component == 0)
			throw 13;

		movement_component->StopMovementImmediately();
		capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		mesh->SetSimulatePhysics(true);

		if (Controller != 0)
			Controller->ChangeState(NAME_Spectating);

		Is_Dead = true;

		SetLifeSpan(Health_Data.Life_Span);
	}
	else
	{
		GetWorldTimerManager().SetTimer(Healing_Timer, this, &AAutomationCharacter::On_Healing, Health_Data.Heal_Rate, true, -1.0);
	}
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::On_Healing()
{
	float heal = Health_Data.Heal_Per_Sec * Health_Data.Heal_Rate;

	Health = FMath::Clamp(Health + heal, 0.0f, Health_Data.Max_Health);
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
//------------------------------------------------------------------------------------------------------------
void AAutomationCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
//------------------------------------------------------------------------------------------------------------
