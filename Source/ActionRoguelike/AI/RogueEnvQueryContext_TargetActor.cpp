// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueEnvQueryContext_TargetActor.h"

#include <gsl/pointers>

#include "AIController.h"
#include "RogueGameTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void URogueEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	APawn* QuerierPawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (ensure(QuerierPawn != nullptr))
	{
		AAIController* Controller = Cast<AAIController>(QuerierPawn->GetController());
    	if (ensure(Controller != nullptr))
    	{
    		AActor* TargetActor = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(NAME_TargetActor));
            
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
    	}	
	}	
}
