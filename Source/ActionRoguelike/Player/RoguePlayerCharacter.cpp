// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp")); 
}


void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
	//Adds a delegate function to the specific attributer listener
	FOnAttributeChanged& Event = ActionSystemComponent->GetAttributeListener(SharedGameplayTags::Attribute_Health);
	Event.AddUObject(this, &ThisClass::OnHealthChanged);
	
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	
	EnhancedInput->BindAction(Input_JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
	
	EnhancedInput->BindAction(Input_SprintAction, ETriggerEvent::Started, this, &ThisClass::StartAction, SharedGameplayTags::Action_Sprint.GetTag());
	EnhancedInput->BindAction(Input_SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopAction, SharedGameplayTags::Action_Sprint.GetTag());
		
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_PrimaryAttack.GetTag());
	EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_SecondaryAttack.GetTag());
	EnhancedInput->BindAction(Input_TeleportAttack, ETriggerEvent::Triggered, this, &ThisClass::StartAction, SharedGameplayTags::Action_SpecialAttack.GetTag());
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0;
	
	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Side to side strafe
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::OnHealthChanged(FGameplayTag AttributeTag, float NewHealth, float OldHealth)
{
	// Died?
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontoge);
	}
}

void ARoguePlayerCharacter::StartAction(FGameplayTag InActionName)
{
	ActionSystemComponent->StartAction(InActionName);
}

void ARoguePlayerCharacter::StopAction(FGameplayTag InActionName)
{
	ActionSystemComponent->StopAction(InActionName);
}


float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                        class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_RageAmount, 5, Modifier);
	
	return ActualDamage;
}





