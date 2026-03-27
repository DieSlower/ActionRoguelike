// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "SharedGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100);
	HealthMax = FRogueAttribute(Health.GetValue());
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.GetValue(), 0, HealthMax.GetValue());
}

URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
	MoveSpeedMultiplier = FRogueAttribute(2.5);
	
}

void URoguePawnAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	
	ApplyMoveSpeed();
}

void URoguePawnAttributeSet::ApplyMoveSpeed()
{
	ACharacter* OwningCharecter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	OwningCharecter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue();
}

URoguePlayerAttributeSet::URoguePlayerAttributeSet()
{
	RageAmount = FRogueAttribute(0);
	RageAmountMax = FRogueAttribute(25);	
}

void URoguePlayerAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	
	ApplyRageAmount();
}

void URoguePlayerAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	
	ApplyRageAmount();
}

void URoguePlayerAttributeSet::ApplyRageAmount()
{
	// Clamp the Rage Amount to a set range 
	RageAmount.Modifier = FMath::Clamp(RageAmount.GetValue(), 0, RageAmountMax.GetValue());
	
	// Get the Action System Comp
	ACharacter* OwningCharecter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	URogueActionSystemComponent* ActionSystemComponent = Cast<URogueActionSystemComponent>(OwningCharecter->GetComponentByClass(URogueActionSystemComponent::StaticClass()));
	
	if (RageAmount.GetValue() == RageAmountMax.GetValue() && ActionSystemComponent->ActiveGameplayTags.HasTag(SharedGameplayTags::StatusEffect_RageBlock))
	{	
		//Remove the Rage Status from abilities
		ActionSystemComponent->ActiveGameplayTags.RemoveTag(SharedGameplayTags::StatusEffect_RageBlock);	
		UE_LOGFMT(LogTemp, Warning, "Removing StatusEffect_RageBlock Tag");
	}
	else if (RageAmount.GetValue() == 0 && !ActionSystemComponent->ActiveGameplayTags.HasTag(SharedGameplayTags::StatusEffect_RageBlock))
	{	
		//Add the Rage Block Status to abilities
		ActionSystemComponent->ActiveGameplayTags.AddTag(SharedGameplayTags::StatusEffect_RageBlock);	
		UE_LOGFMT(LogTemp, Warning, "Adding StatusEffect_RageBlock Tag");		
	}
	else
	{
		UE_LOGFMT(LogTemp, Warning, "Rage Amount Value {0}", RageAmount.GetValue());
	}
}

URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(450);
}
