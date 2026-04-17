// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "SharedGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"


// Sets default values
ARogueAICharacter::ARogueAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp")); 
	ActionSystemComponent->SetDefaultAttributeSet(URogueMonsterAttributeSet::StaticClass());
}

void ARogueAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARogueAICharacter::OnHealthChanged);
}

void ARogueAICharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	UE_LOGFMT(LogTemp, Log, "AI Health Changed from : {0}} to {1}", OldHealth, NewHealth);
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                    class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	
	return ActualDamage;	
}

