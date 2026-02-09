// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTService_CheckHealth.h"

#include "AIController.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URogueBTService_CheckHealth::URogueBTService_CheckHealth()
{
	OwnerActorKey.SelectedKeyName = "SelfActor";
	
	MaxHealthKey.SelectedKeyName = "MaxHealth";	
	CurrentHealthKey.SelectedKeyName = "CurrentHealth";
	IsHealthyKey.SelectedKeyName = "IsHealthy";
}

void URogueBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
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
		
		BBComp->SetValueAsFloat(CurrentHealthKey.SelectedKeyName, ActionComp->GetHealth());
		BBComp->SetValueAsFloat(MaxHealthKey.SelectedKeyName, ActionComp->GetMaxHealth());
		BBComp->SetValueAsBool(IsHealthyKey.SelectedKeyName, ActionComp->IsHealthy());
	}
	
}


