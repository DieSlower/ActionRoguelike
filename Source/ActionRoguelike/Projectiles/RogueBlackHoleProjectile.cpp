// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBlackHoleProjectile.h"

#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
ARogueBlackHoleProjectile::ARogueBlackHoleProjectile()
{
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment(SphereComponent);
	
	SphereComponent->SetSphereRadius(10.f);
	SphereComponent->SetCollisionProfileName("BlackHole");
	
	ProjectileMovementComponent->InitialSpeed=500.f;
	
	GravityForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("GravityForceComp"));
	GravityForceComponent->SetupAttachment(SphereComponent);
	GravityForceComponent->bAutoActivate = true;
	GravityForceComponent->bIgnoreOwningActor = true;
	GravityForceComponent->Radius=1200;
	GravityForceComponent->ForceStrength = -900000;
	GravityForceComponent->DestructibleDamage = 100;
	GravityForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	
}

void ARogueBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARogueBlackHoleProjectile::OnComponentBeginOverlap);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueBlackHoleProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, "OverlapAct: " + OtherActor->GetActorNameOrLabel());
	//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Red, "OverlapCom: " + OtherComp->GetName());
	
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
	}
			
}

void ARogueBlackHoleProjectile::DestroyProjectile()
{
	//GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, "Black Hole Dead");
	Destroy();
}

void ARogueBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle LifeTimerHandle;
	const float LifeTime = 6.f;
	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ARogueBlackHoleProjectile::DestroyProjectile, LifeTime);
}



