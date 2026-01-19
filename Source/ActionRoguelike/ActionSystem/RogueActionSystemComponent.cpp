// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


// Sets default values for this component's properties
URogueActionSystemComponent::URogueActionSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URogueActionSystemComponent::ApplyHealthChange(float healthChange)
{
	float OldHealth = Attributes.Health;
	Attributes.Health += healthChange;
	
	OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
	
	UE_LOG(LogTemp, Log, TEXT("New Health: %f"), Attributes.Health);
}



