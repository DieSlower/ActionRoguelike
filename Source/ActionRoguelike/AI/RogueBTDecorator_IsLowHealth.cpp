// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_IsLowHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
		
	URogueActionSystemComponent* ActionComp = Cast<URogueActionSystemComponent>(Pawn->GetComponentByClass(URogueActionSystemComponent::StaticClass()));
	if (ensure(ActionComp))
	{
		return (ActionComp->GetHealth()/ActionComp->GetMaxHealth()) < LowHealthFraction;
	}
	
	return false;
}
