// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "RogueActionSystemComponent.h"

void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogTemp, Log, "Started Action {ActionName} - {WorldTime}", 
		("ActionName", ActionName.GetTagName()), 
		("WorldTime", GameTime));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogTemp, Log, "Stopped Action {ActionName} - {WorldTime}", 
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
		UE_LOGFMT(LogTemp, Log, "Cooldown Remaining {Time}", GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;	
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
