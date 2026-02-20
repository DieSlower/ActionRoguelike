// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

class URogueAction;

USTRUCT(Blueprintable)
struct FRogueAttributeSet
{
	GENERATED_BODY()
	
	FRogueAttributeSet(): 
	MaxHealth(100), 
	Health(MaxHealth) {}
	
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;
	
	UPROPERTY(BlueprintReadOnly)
	float Health;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, OldHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChanged, float, NewMaxHealth, float, OldMaxHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
	FRogueAttributeSet Attributes;
	
	UPROPERTY(BlueprintReadOnly, Category="Health")
	float HealthyLimit = 30;
	
	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChanged OnMaxHealthChanged;

	// Sets default values for this component's properties
	URogueActionSystemComponent();

	virtual void InitializeComponent();
	
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	void StartAction(FName InActionName);
	
	void ApplyCHealthChange();
	void ApplyCMaxHealthChange();
	
	void ApplyHealthChange(float healthChange);
	void ApplyMaxHealthChange(float maxHealthChange);
	
	float GetMaxHealth() const;
	float GetHealth() const;
	bool IsFullHealth() const;
	bool IsHealthy() const;
};
