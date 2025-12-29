// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBlackHoleProjectile.h"

#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ARogueBlackHoleProjectile::ARogueBlackHoleProjectile()
{
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment((SphereComponent));
	
	SphereComponent->SetSphereRadius(260.f);
	
	ProjectileMovementComponent->InitialSpeed=500.f;
}

void ARogueBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARogueBlackHoleProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
	
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, "Hit: " + OtherActor->GetActorNameOrLabel());
}

void ARogueBlackHoleProjectile::DestroyProjectile()
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Green, "Black Hole Dead");
	
	Destroy();
}

void ARogueBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle LifeTimerHandle;
	const float LifeTime = 6.f;
	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ARogueBlackHoleProjectile::DestroyProjectile, LifeTime);
}



