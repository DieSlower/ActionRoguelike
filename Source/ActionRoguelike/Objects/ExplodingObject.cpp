// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingObject.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ActionRoguelike/Projectiles/RogueProjectileMagic.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ProfilingDebugging/CookStats.h"


// Sets default values
AExplodingObject::AExplodingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->SetSimulatePhysics(true);

	ExplosionForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionForceComp"));
	ExplosionForceComponent->SetupAttachment((MeshComponent));
	ExplosionForceComponent->bAutoActivate = false;
	ExplosionForceComponent->bIgnoreOwningActor = true;
	ExplosionForceComponent->ImpulseStrength=50000;
	ExplosionForceComponent->Radius=100000;
	ExplosionForceComponent->DestructibleDamage = 100;
}

void AExplodingObject::Explode()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionAnimation, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionAftermathSound, GetActorLocation(), FRotator::ZeroRotator);
	
	ExplosionForceComponent->FireImpulse();
	
	MeshComponent->AddImpulse(FVector::UpVector * 1000, NAME_None, true);
	MeshComponent->AddAngularImpulseInDegrees(FVector::UpVector * 10000, NAME_None, true);
}

void AExplodingObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

float AExplodingObject::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (GetWorldTimerManager().TimerExists(mDeathTimerHandle))
	{
		return ActualDamage;
	}
	
	mTotalDamage += DamageAmount;
	
	if ( DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{	
		const FPointDamageEvent* pointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		UNiagaraFunctionLibrary::SpawnSystemAttached(FireAnimation, MeshComponent, "FireLocation",
													 pointDamageEvent->HitInfo.ImpactPoint, FRotator::ZeroRotator, EAttachLocation::Type::KeepWorldPosition, true );
	}
			
	if (mTotalDamage >= 30.0f && !mDeathTimerSet)
	{
		GetWorldTimerManager().SetTimer(mDeathTimerHandle, this, &AExplodingObject::DeathTimerElapsed, mDeathDelayTime);	
		mDeathTimerSet = true;
		
		FString TheFloatStr = FString::SanitizeFloat(mTotalDamage);
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, "Damage: " + TheFloatStr);
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, "Exploding....");
	}
	
	return ActualDamage;
}

// Called when the game starts or when spawned
void AExplodingObject::BeginPlay()
{
	Super::BeginPlay();	
}

void AExplodingObject::DestroyTimerElapsed()
{
	Destroy();
}

void AExplodingObject::DeathTimerElapsed()
{
	Explode();
	
	GetWorldTimerManager().SetTimer(mDestroyTimerHandle, this, &AExplodingObject::DestroyTimerElapsed, mDestroyDelayTime);	
}
