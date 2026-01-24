// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupbleObject.h"
#include "HealthPickupObject.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AHealthPickupObject : public APickupbleObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPickupObject();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Health")
	float HealAmount = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Health")
	TSubclassOf<UDamageType> HealthClass;
	
	virtual void OnComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
};
