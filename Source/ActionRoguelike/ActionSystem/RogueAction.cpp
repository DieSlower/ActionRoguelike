// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "ActionRoguelike.h"
#include "RogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogGame, Log, "Started Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.GetTagName()), 
		("WorldTime", GameTime));
	
	URogueActionSystemComponent* OwningComponent = GetOwningComponent();
	OwningComponent->ActiveGameplayTags.AppendTags(GrantTags);
	
	// Consume required resources
	for (TPair<FGameplayTag,float> Cost : ActivationCost)
	{
		OwningComponent->ApplyAttributeChange(Cost.Key, -Cost.Value, Modifier);
	}
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogGame, Log, "Stopped Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.GetTagName()), 
		("WorldTime", GameTime));
	
	CooldownUntil = GetWorld()->GetTimeSeconds() + CooldownTime;
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
}

bool URogueAction::CanStart() const
{
	if (IsRunning())
	{
		return false;
	}
	
	if (GetCooldownTimeRemaining() > 0.0f)
	{
		UE_LOGFMT(LogGame, Log, "Cooldown Remaining {Time}", GetCooldownTimeRemaining());
		return false;
	}
	
	URogueActionSystemComponent* OwningComponent = GetOwningComponent();
	if (OwningComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;	
	}
	
	// Check if we have enough "cost" to use this action
	for (TPair<FGameplayTag,float> Cost : ActivationCost)
	{
		if (Cost.Key.GetTagName() == "None")
		{
			UE_LOGFMT(LogGame, Error, "Empty Activation Cost Present in {0}", ActionName.GetTagName());
			return false;
		}
		float AvailableAttributeAmount = OwningComponent->GetAttributeValue(Cost.Key);
		if (AvailableAttributeAmount < Cost.Value)
		{
			UE_LOGFMT(LogGame, Warning, "Not enough {AttributeName} to activate {ActionName}."
							"Have {AvailableAttributeValue} and need {RequiredAttributeValue}.",
							("AttributeName", Cost.Key.ToString()),
							("ActionName", ActionName.ToString()),
							("AvailableAttributeValue", AvailableAttributeAmount),
							("RequiredAttributeValue", Cost.Value));
			return false;
		}
	}
	
	return true;
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}

float URogueAction::GetCooldownTimeRemaining() const
{
	return FMath::Max(0.0f, CooldownUntil - GetWorld()->GetTimeSeconds());
}
