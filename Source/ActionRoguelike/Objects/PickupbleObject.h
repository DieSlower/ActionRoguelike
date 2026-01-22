// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupbleObject.generated.h"

UCLASS()
class ACTIONROGUELIKE_API APickupbleObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupbleObject();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UMeshComponent> MeshComponent;
};
