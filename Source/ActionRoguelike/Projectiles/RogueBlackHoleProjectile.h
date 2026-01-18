// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueBlackHoleProjectile.generated.h"

class URadialForceComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueBlackHoleProjectile : public ARogueProjectile
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<URadialForceComponent> GravityForceComponent;
	
	// Sets default values for this actor's properties
	ARogueBlackHoleProjectile();
	
	virtual void PostInitializeComponents() override;
	
protected:
	UFUNCTION()
	virtual void OnComponentBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	void DestroyProjectile();
	
	virtual void BeginPlay() override;
};
