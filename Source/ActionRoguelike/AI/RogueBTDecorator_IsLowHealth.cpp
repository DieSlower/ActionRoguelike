// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_IsLowHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "SharedGameplayTags.h"

bool URogueBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	
	check(Pawn)
		
	URogueActionSystemComponent* ActionComp = Cast<URogueActionSystemComponent>(Pawn->GetComponentByClass(URogueActionSystemComponent::StaticClass()));
	if (ensure(ActionComp))
	{
		float HealthFraction = ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_Health) / ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_HealthMax);
		// Is low health?
		return HealthFraction < LowHealthFraction;
	}
	
	return false;
}
