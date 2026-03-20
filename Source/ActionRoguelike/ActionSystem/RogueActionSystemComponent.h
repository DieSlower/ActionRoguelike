// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RogueAttributeSet.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

class URogueAction;
class URogueAttributeSet;

UENUM(BlueprintType)
enum EAttributeModifyType
{
	Base,
	Modifier,
	OverrideBase,
	Invalid
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChanged, float, NewMaxHealth, float, OldMaxHealth);

// Native c++ delegate
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/,  float /*OldAttributeValue*/);
// BP Delegate
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeDynamicChanged, FGameplayTag, AttributeTag, float ,NewAttributeValue,  float, OldAttributeValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	TObjectPtr<URogueAttributeSet> Attributes;
	
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
		
	UPROPERTY(EditAnywhere, Category="Attributes", NoClear)
	TSubclassOf<URogueAttributeSet> AttributeSetClass;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
	TMap<FGameplayTag, TArray<FOnAttributeDynamicChanged>> AttributeDynamicListeners;
	
	UPROPERTY(BlueprintReadOnly, Category="Health")
	float HealthyLimit = 30;
	
	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<URogueAction>> DefaultActions;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnMaxHealthChanged OnMaxHealthChanged;

	FGameplayTagContainer ActiveGameplayTags;
	
	// Sets default values for this component's properties
	URogueActionSystemComponent();

	FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName="Add Attribute Listener Event", meta = (keywords="events, delegate, listener"))
	void AddDynamicAttributeListener(FOnAttributeDynamicChanged Event, FGameplayTag AttributeTag);
	
	virtual void InitializeComponent();
	
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	virtual void BeginPlay() override;

	void StartAction(FGameplayTag InActionName);
	void StopAction(FGameplayTag InActionName);
	
	void ApplyCHealthChange();
	void ApplyCMaxHealthChange();
	
	//void ApplyHealthChange(float healthChange);
	void ApplyMaxHealthChange(float maxHealthChange);
	
	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType);
	
	FRogueAttribute* GetAttribute(FGameplayTag InAttributeTag) const;
	
	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(FGameplayTag InAttributeTag) const;
	
	bool IsHealthy() const;
};
