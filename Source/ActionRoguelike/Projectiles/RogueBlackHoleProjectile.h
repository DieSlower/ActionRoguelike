// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueBlackHoleProjectile.generated.h"

class UNiagaraComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueBlackHoleProjectile : public ARogueProjectile
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;
	
	// Sets default values for this actor's properties
	ARogueBlackHoleProjectile();
	
	virtual void PostInitializeComponents() override;
	
protected:
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit ) override;
	
	void DestroyProjectile();
	
	virtual void BeginPlay() override;
};
