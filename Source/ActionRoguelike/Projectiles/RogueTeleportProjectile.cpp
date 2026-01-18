// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueTeleportProjectile.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARogueTeleportProjectile::ARogueTeleportProjectile()
{
	
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment((SphereComponent));	
}

// Called when the game starts or when spawned
void ARogueTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARogueTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
		
	Teleport();
}

void ARogueTeleportProjectile::Teleport()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	// Actor->TeleportTo(…)
	
	Destroy();
}


