// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAction.h"
#include "RogueAction_ProjectileAttack.generated.h"

class ARogueProjectile;
class UNiagaraSystem;
/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API URogueAction_ProjectileAttack : public URogueAction
{
	GENERATED_BODY()
	
public:
	URogueAction_ProjectileAttack();
	
	virtual void StartAction_Implementation() override;
	
	void AttackTimerElapsed();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TSubclassOf<ARogueProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="ProjectileAttack")
	TObjectPtr<UAnimMontage> AttackMontoge;
	
	
	
};
