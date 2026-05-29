// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "ActionRoguelike.h"
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
	
	//Make sure the overlay is always culled out
	GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
	
	//ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARogueAICharacter::OnHealthChanged);
}

void ARogueAICharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	UE_LOGFMT(LogGame, Log, "AI Health Changed from : {0}} to {1}", OldHealth, NewHealth);
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                    class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	
	
	//Make sure the overlay is never culled out
	GetMesh()->SetOverlayMaterialMaxDrawDistance(0);
	
	//Using Material Instance Dynamic, esier because it is by name, but more expensive to scale. 
	//GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	
	// Using Custom Primitive Data, scales better but can conflict with index numbers. 
	// Can get an index clash if multiple materials have the same index 0
	// Changes Index 0, in this case on the overlay material. 
	GetMesh()->SetCustomPrimitiveDataFloat(0, GetWorld()->TimeSeconds);
	
	GetWorldTimerManager().SetTimer(OverlayTimerHandle, [this]()
	{
		//Make sure the overlay is always culled out
		GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
	}, 1.f, false);
	
	return ActualDamage;	
}

