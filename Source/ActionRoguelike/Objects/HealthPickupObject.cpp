// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickupObject.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Player/RoguePlayerCharacter.h"


// Sets default values
AHealthPickupObject::AHealthPickupObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHealthPickupObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARoguePlayerCharacter* Player = Cast<ARoguePlayerCharacter>(OtherActor);
	check(Player);
	URogueActionSystemComponent* ActionComp = Player->FindComponentByClass<URogueActionSystemComponent>();
	
	//Assert if ActionComp is null. All pawns should have an action component
	//Skip pickup if the health is full
	if (ensure(ActionComp != nullptr) && !ActionComp->IsFullHealth())
	{
		Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		ActionComp->ApplyHealthChange(HealAmount);
		Destroy();
	}
}



