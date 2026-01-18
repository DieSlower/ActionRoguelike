// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueTeleportProjectile.generated.h"

class UNiagaraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueTeleportProjectile : public ARogueProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARogueTeleportProjectile();
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> PortalCloseEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> PortalCloseSound;
	
protected:
	FTimerHandle LifeTimerHandle;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit ) override;
	
	void LifeTimerElapsed();
	
	void StartTeleport();
	void CompleteTeleport();
};
