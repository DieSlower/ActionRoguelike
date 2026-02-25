// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "RogueAction.h"
#include "RogueAttributeSet.h"

float defaultVal = 686868.f;
TAutoConsoleVariable<float> CVarAddMaxHealth(TEXT("game.health.AddMaxHealth"), defaultVal, TEXT("Add Max Health to the player (Can be positive or negative)"), ECVF_Cheat);
TAutoConsoleVariable<float> CVarAddHealth(TEXT("game.health.AddHealth"), defaultVal, TEXT("Add to player Health (Can be positive or negative)"), ECVF_Cheat);

// Sets default values for this component's properties
URogueActionSystemComponent::URogueActionSystemComponent()
{
	bWantsInitializeComponent = true;

	AttributeSetClass = URogueAttributeSet::StaticClass();
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
		ApplyHealthChange(healthChange);		
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

void URogueActionSystemComponent::ApplyHealthChange(float healthChange)
{
	/*float OldHealth = Attributes.Health;
	float MaxHealth = Attributes.MaxHealth;
	
	Attributes.Health = FMath::Clamp(Attributes.Health += healthChange, 0.f, MaxHealth);
	
	if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
	{
		OnHealthChanged.Broadcast(Attributes.Health, OldHealth);	
	}
		
	UE_LOG(LogTemp, Log, TEXT("New Health: %f of %f"), Attributes.Health, MaxHealth);*/
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

FRogueAttribute* URogueActionSystemComponent::GetAttribute(FGameplayTag InAttributeTag) const
{
	FRogueAttribute* FoundAttribute = *CachedAttributes.Find(InAttributeTag);
	
	return FoundAttribute;
}

bool URogueActionSystemComponent::IsFullHealth() const
{
	/*if (FMath::IsNearlyEqual(Attributes.Health,  Attributes.MaxHealth))
	{
		return true;
	}
	else
	{
		return false;
	}*/
	
	return true;
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



