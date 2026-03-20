// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "RogueAction.h"
#include "RogueAttributeSet.h"
#include "SharedGameplayTags.h"

float defaultVal = 686868.f;
TAutoConsoleVariable<float> CVarAddMaxHealth(TEXT("game.health.AddMaxHealth"), defaultVal, TEXT("Add Max Health to the player (Can be positive or negative)"), ECVF_Cheat);
TAutoConsoleVariable<float> CVarAddHealth(TEXT("game.health.AddHealth"), defaultVal, TEXT("Add to player Health (Can be positive or negative)"), ECVF_Cheat);

// Sets default values for this component's properties
URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;
	
	// Enable Component Tick to make sure the CVars work -- needs to be reworked. 
	PrimaryComponentTick.bCanEverTick = true;
	
	AttributeSetClass = URogueAttributeSet::StaticClass();
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag AttributeTag)
{
	return AttributeListeners.FindOrAdd(AttributeTag);
}

void URogueActionSystemComponent::AddDynamicAttributeListener(FOnAttributeDynamicChanged Event,	FGameplayTag AttributeTag)
{
	TArray<FOnAttributeDynamicChanged>& Events = AttributeDynamicListeners.FindOrAdd(AttributeTag);
	Events.Add(Event);
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Attributes = NewObject<URogueAttributeSet>(this, AttributeSetClass);
	
	for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
	{
		FRogueAttribute* FoundAttribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
		FName AttributeTagName = FName("Attribute." + PropIt->GetName());
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName);
		
		CachedAttributes.Add(AttributeTag, FoundAttribute);
		UE_LOGFMT(LogTemp, Warning, "Size of Cache {Size} Name: {name}", CachedAttributes.GetMaxIndex(), AttributeTagName);
	}
	
	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
	{
		if (ensure(ActionClass))
		{
			GrantAction(ActionClass);
		}
	}
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
 	Actions.Add(NewAction);
}

void URogueActionSystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// These should be run by a timer 1x per second, not in Tick()
	ApplyCHealthChange();
	ApplyCMaxHealthChange();
}

void URogueActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Attributes->InitializeAttributes();
}

void URogueActionSystemComponent::StartAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if(Action->GetActionName() == InActionName)
		{
			if (Action->CanStart())
			{
				Action->StartAction();
			}
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Action Name: %s Not Found!"), *InActionName.ToString());
}

void URogueActionSystemComponent::StopAction(FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if(Action->GetActionName() == InActionName)
		{
			Action->StopAction();
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Action Name: %s Not Found!"), *InActionName.ToString());
}

void URogueActionSystemComponent::ApplyCHealthChange()
{	
	float healthChange = CVarAddHealth.GetValueOnGameThread();
	if (healthChange != defaultVal)
	{
		UE_LOG(LogTemp, Log, TEXT("Health Change: %f"), healthChange);
		
		// Find the console variable by its name
		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("game.health.AddHealth"));

		// Check if the variable was found
		if (CVar)
		{
			// Set the new value, specifying the change source (ECVF_SetByCode gives it a high priority)
			CVar->Set(defaultVal, ECVF_SetByConsole); 
		}
		ApplyAttributeChange(SharedGameplayTags::Attribute_Health, healthChange, Base);
		//UE_LOG(LogTemp, Log, TEXT("New Health: %f of %f"), Attributes.Health, Attributes.MaxHealth);
	}	
}

void URogueActionSystemComponent::ApplyCMaxHealthChange()
{
	float healthChange = CVarAddMaxHealth.GetValueOnGameThread();
	if (healthChange != defaultVal)
	{
		UE_LOG(LogTemp, Log, TEXT("Max Health Change: %f"), healthChange);
		
		// Find the console variable by its name
		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("game.health.AddMaxHealth"));

		// Check if the variable was found
		if (CVar)
		{
			// Set the new value, specifying the change source (ECVF_SetByCode gives it a high priority)
			CVar->Set(defaultVal, ECVF_SetByConsole); 
		}
		ApplyMaxHealthChange(healthChange);		
		//UE_LOG(LogTemp, Log, TEXT("New Max Health: %f of %f"), Attributes.Health, Attributes.MaxHealth);
	}
}

void URogueActionSystemComponent::ApplyMaxHealthChange(float maxHealthChange)
{
	/*float OldMaxHealth = Attributes.MaxHealth;
	float NewMaxHealth = Attributes.MaxHealth += maxHealthChange;
	
	Attributes.MaxHealth = NewMaxHealth;
	
	if (!FMath::IsNearlyEqual(OldMaxHealth, Attributes.MaxHealth))
	{
		OnMaxHealthChanged.Broadcast(Attributes.MaxHealth, OldMaxHealth);	
	}
		
	UE_LOG(LogTemp, Log, TEXT("New Max Health: %f from %f"), Attributes.MaxHealth, OldMaxHealth);*/
}

void URogueActionSystemComponent::ApplyAttributeChange(FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType)
{
	FRogueAttribute* FoundAttribute = GetAttribute(AttributeTag);
	check(FoundAttribute != nullptr);
	
	float OldValue = FoundAttribute->GetValue();
		
	switch (ModifyType)
	{
	case Base:
		FoundAttribute->Base += Delta;
		break;
		
	case Modifier:
		FoundAttribute->Modifier += Delta;
		break;
		
	case OverrideBase:
		FoundAttribute->Base = Delta;
		break;
		
	default:
		check(false);
	}
	
	Attributes->PostAttributeChanged();
	
	// If the attribute has a c++ listener, find it and broadcast it. 
	if (FOnAttributeChanged* Event =  AttributeListeners.Find(AttributeTag))
	{
		Event->Broadcast(AttributeTag, FoundAttribute->GetValue(), OldValue);
	}
	
	// If the attribute has a BP listener, find it and broadcast it.
	if (TArray<FOnAttributeDynamicChanged>* Events = AttributeDynamicListeners.Find(AttributeTag))
	{
		for (FOnAttributeDynamicChanged& Event : *Events)
		{
			Event.Execute(AttributeTag, FoundAttribute->GetValue(), OldValue);
		}	
	}
	
	
	UE_LOGFMT(LogTemp, Log, "Attribute: {0}, New: {1}, Old {2}", AttributeTag.ToString(), FoundAttribute->GetValue(), OldValue);
}

FRogueAttribute* URogueActionSystemComponent::GetAttribute(FGameplayTag InAttributeTag) const
{
	FRogueAttribute* FoundAttribute = *CachedAttributes.Find(InAttributeTag);
	return FoundAttribute;
}

float URogueActionSystemComponent::GetAttributeValue(FGameplayTag InAttributeTag) const
{
	FRogueAttribute* FoundAttribute = GetAttribute(InAttributeTag);
	return FoundAttribute->GetValue();
}

bool URogueActionSystemComponent::IsHealthy() const
{
	/*if (Attributes.Health > HealthyLimit)
	{
		return true;
	}
	else
	{
		return false;
	}*/
	
	return true;
}
