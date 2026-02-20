// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class URogueActionSystemComponent;
class ARogueTeleportProjectile;
class ARogueProjectile;
class UNiagaraSystem;
class ARogueProjectileMagic;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ARoguePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoguePlayerCharacter();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Death")
	TObjectPtr<UAnimMontage> DeathMontoge;
	
	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
	TSubclassOf<ARogueProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(VisibleAnywhere, Category="PrimaryAttack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="PrimaryAttack")
	TObjectPtr<UAnimMontage> AttackMontoge;
	
	UPROPERTY(EditDefaultsOnly, Category="SecondaryAttack")
	TObjectPtr<UNiagaraSystem> Casting2Effect;
	
	UPROPERTY(EditDefaultsOnly, Category="SecondaryAttack")
	TObjectPtr<USoundBase> Casting2Sound;
	
	UPROPERTY(EditDefaultsOnly, Category="SecondaryAttack")
	TSubclassOf<ARogueProjectile> Projectile2Class;
	
	UPROPERTY(VisibleAnywhere, Category="SecondaryAttack")
	FName Muzzle2SocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="SecondaryAttack")
	TObjectPtr<UAnimMontage> Attack2Montoge;
	
	UPROPERTY(EditDefaultsOnly, Category="TeleportAttack")
	TSubclassOf<ARogueProjectile> ProjectileTeleportClass;
	
	UPROPERTY(EditDefaultsOnly, Category="TeleportAttack")
	TObjectPtr<UNiagaraSystem> CastingTeleportEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="TeleportAttack")
	TObjectPtr<USoundBase> CastingTeleportSound;
	
	UPROPERTY(VisibleAnywhere, Category="TeleportAttack")
	FName MuzzleTeleportSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="TeleportAttack")
	TObjectPtr<UAnimMontage> AttackTeleportMontoge;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_TeleportAttack;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	
	UFUNCTION()
	void OnHealthChanged(float NewHealth, float OldHealth);
	
	//void JumpAction();

	void StartAction(FName InActionName);
	
	void PrimaryAttack();
	void SecondaryAttack();
	void SpecialAttack();
	
	void AttackTimerElapsed();
	void Attack2TimerElapsed();
	void AttackTeleportTimerElapsed();
	
	
public:	
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
