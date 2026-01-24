// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickupObject.h"

#include "ActionSystem/RogueActionSystemComponent.h"
#include "Kismet/GameplayStatics.h"
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
	ARoguePlayerCharacter* Player = CastChecked<ARoguePlayerCharacter>(OtherActor);
	URogueActionSystemComponent* ActionComp = Player->FindComponentByClass<URogueActionSystemComponent>();
	
	if (ActionComp->GetMaxHealth() > ActionComp->GetHealth())
	{
		Super::OnComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		UGameplayStatics::ApplyDamage(OtherActor, -100.f, GetInstigatorController(), this, HealthClass);	
		Destroy();
	}
}



