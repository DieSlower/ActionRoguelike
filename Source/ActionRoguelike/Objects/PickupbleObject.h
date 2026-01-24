// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupbleObject.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API APickupbleObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupbleObject();
	
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UMeshComponent> MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> PickupSound;
	
	UFUNCTION()
	virtual void OnComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
