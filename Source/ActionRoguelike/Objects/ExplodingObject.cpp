// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingObject.h"

#include "ActionRoguelike/Projectiles/RogueProjectileMagic.h"
#include "Components/SphereComponent.h"


// Sets default values
AExplodingObject::AExplodingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(true);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName("PhysicsActor");
	MeshComponent->SetSimulatePhysics(true);
	//this->OnTakeAnyDamage
}

void AExplodingObject::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{		
	
}

void AExplodingObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	MeshComponent->OnComponentHit.AddDynamic(this, &AExplodingObject::OnActorHit);
}

float AExplodingObject::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 	class AController* EventInstigator, AActor* DamageCauser)
{
	mTotalDamage += DamageAmount;
	
	FString TheFloatStr = FString::SanitizeFloat(mTotalDamage);
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow, TheFloatStr);	
	
	if (mTotalDamage >= 30.0f)
	{
		FTimerHandle DeathTimerHandle;
		const float DeathDelayTime = 5.f;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AExplodingObject::DeathTimerElapsed, DeathDelayTime);	
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
	Destroy();
}
