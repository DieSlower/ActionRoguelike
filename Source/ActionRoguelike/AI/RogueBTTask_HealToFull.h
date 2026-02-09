// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RogueBTTask_HealToFull.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTTask_HealToFull : public UBTTaskNode
{
	GENERATED_BODY()
public:
	URogueBTTask_HealToFull();
		
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector OwnerActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
