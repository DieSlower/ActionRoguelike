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
		
	const float LifeTime = 3.f;
	GetWorldTimerManager().SetTimer(LifeTimerHandle, this, &ARogueTeleportProjectile::LifeTimerElapsed, LifeTime);
}

void ARogueTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
		
	StartTeleport();
}

void ARogueTeleportProjectile::LifeTimerElapsed()
{
	StartTeleport();
}

void ARogueTeleportProjectile::StartTeleport()
{
	GetWorldTimerManager().ClearTimer(LifeTimerHandle);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	
	FTimerHandle PortalTimerHandle;
	const float LifeTime = 0.2f;
	GetWorldTimerManager().SetTimer(PortalTimerHandle, this, &ARogueTeleportProjectile::CompleteTeleport, LifeTime);
}

void ARogueTeleportProjectile::CompleteTeleport()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PortalCloseEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, PortalCloseSound, GetActorLocation(), FRotator::ZeroRotator);
	
	APawn* MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	MyCharacter->TeleportTo(GetActorLocation(), MyCharacter->GetActorRotation());
	
	Destroy();
}


