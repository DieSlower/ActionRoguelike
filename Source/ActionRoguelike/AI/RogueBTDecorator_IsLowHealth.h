// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RogueBTDecorator_IsLowHealth.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTDecorator_IsLowHealth : public UBTDecorator
{
	GENERATED_BODY()
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	
protected:
	
	UPROPERTY(EditAnywhere, Category="AI", meta=(ClampMin="0.0", ClampMax="1"))
	float LowHealthFraction = 0.3f;	
};
