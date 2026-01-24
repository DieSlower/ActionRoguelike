// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupbleObject.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickupbleObject::APickupbleObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->SetCollisionProfileName("PickupbleObject");
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment((SphereComponent));
	MeshComponent->SetCollisionProfileName("PickupbleObject");
}

void APickupbleObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
		
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupbleObject::OnComponentBeginOverlap);
}

void APickupbleObject::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
}


