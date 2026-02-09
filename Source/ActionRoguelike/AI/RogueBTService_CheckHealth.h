// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RogueBTService_CheckHealth.generated.h"

class URogueActionSystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
	
public:
	URogueBTService_CheckHealth();

protected:
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector OwnerActorKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector MaxHealthKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector CurrentHealthKey;
	
	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector IsHealthyKey;
	
	//TSubclassOf<URogueActionSystemComponent> ActionSystemComp;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
