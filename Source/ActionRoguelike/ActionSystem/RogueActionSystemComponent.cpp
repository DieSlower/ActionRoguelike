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
	
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag AttributeTag)
{
	// Bind to a c++ listener delegate
	return AttributeListeners.FindOrAdd(AttributeTag);
}

void URogueActionSystemComponent::AddDynamicAttributeListener(FOnAttributeDynamicChanged Event,	FGameplayTag AttributeTag)
{
	// Bind to a BP listener delegate
	TArray<FOnAttributeDynamicChanged>& Events = AttributeDynamicListeners.FindOrAdd(AttributeTag);
	Events.Add(Event);
}

void URogueActionSystemComponent::RemoveDynamicAttributeListener(FOnAttributeDynamicChanged Event)
{
	for (TPair<FGameplayTag, TArray<FOnAttributeDynamicChanged>>& Listener : AttributeDynamicListeners)
	{
		if (Listener.Value.RemoveSingle(Event) > 0)
		{
			UE_LOGFMT(LogTemp, Warning, "Removed BP Binding");
			break;
		}
	}
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Fallback check in case the user did not set an Attribute set in code or BPs
	if (Attributes == nullptr)
	{
		Attributes = NewObject<URogueAttributeSet>(this, URogueAttributeSet::StaticClass());
		UE_LOGFMT(LogTemp, Warning, "No default AttributeSet defined. Set using SetDefaultAttributeSet() during Actor Construction or assign in Blueprint Action Component for {0}", GetNameSafe(GetOwner()));
	}
	
	for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
	{
		FRogueAttribute* FoundAttribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
		FName AttributeTagName = FName("Attribute." + PropIt->GetName());
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName);
		
		CachedAttributes.Add(AttributeTag, FoundAttribute);
	}
	
	for (TSubclassOf<URogueAction> ActionClass : DefaultActions)
	{
		if (ensure(ActionClass))
		{
			GrantAction(ActionClass);
		}
	}
}

void URogueActionSystemComponent::SetDefaultAttributeSet(TSubclassOf<URogueAttributeSet> AttributeSetClass)
{
	check(!HasBeenInitialized())
	
	// Only available in constructors of UObjects
	FObjectInitializer& ObjectInitializer = FObjectInitializer::Get();
	
	// Creates the default attribute set for this actor
	Attributes = Cast<URogueAttributeSet>(ObjectInitializer.CreateDefaultSubobject(this, TEXT("Attributes"), AttributeSetClass, AttributeSetClass));
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
 	Actions.Add(NewAction);
}

void URogueActionSystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// These should be run by a timer 1x per second, not in Tick(). Tick=BAD
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
			// Set the default "check" value back, specifying the change source (ECVF_SetByCode gives it a high priority)
			CVar->Set(defaultVal, ECVF_SetByConsole); 
			
			// Set the new value through a delegate
			ApplyAttributeChange(SharedGameplayTags::Attribute_Health, healthChange, Base);
		}
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
			// Set the default "check" value back,, specifying the change source (ECVF_SetByCode gives it a high priority)
			CVar->Set(defaultVal, ECVF_SetByConsole);
			
			// Set the new value through a delegate
			ApplyAttributeChange(SharedGameplayTags::Attribute_HealthMax, healthChange, OverrideBase);
		}
	}
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
		bool bIsBound = true;		
		for (int i = Events->Num() - 1; i >= 0; --i)
		{
			FOnAttributeDynamicChanged& Event = (*Events)[i];
			bIsBound = Event.ExecuteIfBound(AttributeTag, FoundAttribute->GetValue(), OldValue);
			if (!bIsBound) // Delete listener if nothing is bound to it anymore.
			{
				Events->RemoveAt(i);
				UE_LOGFMT(LogTemp, Log, "Cleaned up expired attribute delegate for {0}", GetNameSafe(GetOwner()));
			}
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
