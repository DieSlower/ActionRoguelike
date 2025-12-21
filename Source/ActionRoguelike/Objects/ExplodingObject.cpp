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
	ExplosionForceComponent->ImpulseStrength=500000;
	ExplosionForceComponent->Radius=100000;
	ExplosionForceComponent->DestructibleDamage = 100;
}

void AExplodingObject::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, "Hit");	
}

void AExplodingObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &AExplodingObject::OnActorHit);
}

float AExplodingObject::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 	class AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, "Damage");
	mTotalDamage += DamageAmount;
	
	FString TheFloatStr = FString::SanitizeFloat(mTotalDamage);
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, TheFloatStr);	
	
	TheFloatStr = FString::SanitizeFloat(DamageEvent.GetTypeID());
		GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::White, TheFloatStr);
	
	if ( DamageEvent.IsOfType(1))
	{	
		const FPointDamageEvent* pointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		UNiagaraFunctionLibrary::SpawnSystemAttached(FireAnimation, MeshComponent, "FireLocation",
													 pointDamageEvent->HitInfo.ImpactPoint, FRotator::ZeroRotator, EAttachLocation::Type::KeepWorldPosition, true );
	}
			
	if (mTotalDamage >= 30.0f && !mDeathTimerSet)
	{
		FTimerHandle DeathTimerHandle;
		const float DeathDelayTime = 5.f;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AExplodingObject::DeathTimerElapsed, DeathDelayTime);	
		mDeathTimerSet = true;
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called when the game starts or when spawned
void AExplodingObject::BeginPlay()
{
	Super::BeginPlay();	
}

void AExplodingObject::DeathTimerElapsed()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionAnimation, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionAftermathSound, GetActorLocation(), FRotator::ZeroRotator);
	
	ExplosionForceComponent->FireImpulse();
	
	Destroy();
}
