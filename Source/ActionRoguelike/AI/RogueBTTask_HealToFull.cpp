// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTTask_HealToFull.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URogueBTTask_HealToFull::URogueBTTask_HealToFull()
{
	OwnerActorKey.SelectedKeyName = "SelfActor";
}

EBTNodeResult::Type URogueBTTask_HealToFull::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	check(BBComp);
	
	AActor* OwnerActor = Cast<AActor>( BBComp->GetValueAsObject(OwnerActorKey.SelectedKeyName));
	if (OwnerActor)
	{
		AAIController* Controller = OwnerComp.GetAIOwner();
		
		ACharacter* OwnerCharacter = Controller->GetCharacter(); 
		check(OwnerCharacter);	
				
		URogueActionSystemComponent* ActionComp = Cast<URogueActionSystemComponent>(OwnerCharacter->GetComponentByClass(URogueActionSystemComponent::StaticClass()));
		check(ActionComp);
		
		ensure(false);
		//ActionComp->ApplyHealthChange(ActionComp->GetMaxHealth());	
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
