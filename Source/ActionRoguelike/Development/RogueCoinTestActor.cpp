// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinTestActor.h"

#include "NavigationSystem.h"
#include "Pickups/RogueCoinPickupSubsystem.h"

ARogueCoinTestActor::ARogueCoinTestActor()
{
	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComp;
}

void ARogueCoinTestActor::SpawnCoins(int32 SpawnCount)
{
	
	TArray<FVector> CoinLocations;
	TArray<int32> CoinAmounts;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	FVector ActorLocation = GetActorLocation();
	
	for (int i=0; i<SpawnCount; ++i)
	{
		// Generate a random point that is on a navigation mesh for our use
		FNavLocation NavLocation;
		NavSystem->GetRandomPointInNavigableRadius(ActorLocation, 1024, NavLocation);
		
		//Add the new point and it's coin amount to our data arrays. 
		CoinLocations.Add(NavLocation);
		CoinAmounts.Add(10);	
		
	}
	
	URogueCoinPickupSubsystem* CoinSystem = GetWorld()->GetSubsystem<URogueCoinPickupSubsystem>();
	
	CoinSystem->AddCoinPickups(CoinLocations, CoinAmounts);
}


