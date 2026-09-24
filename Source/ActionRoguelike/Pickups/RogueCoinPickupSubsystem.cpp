// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

#include "ActionRoguelike.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/URogueDeveloperSettings.h"
#include "Player/RoguePlayerCharacter.h"
#include "ProfilingDebugging/CountersTrace.h"

TRACE_DECLARE_INT_COUNTER(CoinInstanceCount, TEXT("Coins in World"))

void URogueCoinPickupSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UWorld* World = GetWorld();
	
	//Temp Hack for mesh loading
	//FSoftObjectPath MeshAssetPath(TEXT("/Game/ExampleContent/Meshes/SM_Pickup_Coin.SM_Pickup_Coin"));
	//UStaticMesh* LoadedMesh = Cast<UStaticMesh>( MeshAssetPath.TryLoad());
	
	
	
	WorldISM = NewObject<UInstancedStaticMeshComponent>(World, NAME_None, RF_Transient);
	WorldISM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WorldISM->RegisterComponentWithWorld(World);
	
	TRACE_COUNTER_SET(CoinInstanceCount, 0);
	
	const URogueDeveloperSettings* DevSettings = GetDefault<URogueDeveloperSettings>();
	
	//Get the triger name from settings
	CoinPickupTriggerParameterName = DevSettings->CoinPickupTriggerParameter;
	
	//Make an async delegate to load the static mesh
	DevSettings->CoinPickupMesh.LoadAsync(
		FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &URogueCoinPickupSubsystem::OnPickupMeshLoadComplete)
		);
	
	WorldAudioComp = NewObject<UAudioComponent>(World, NAME_None, RF_Transient);
	WorldAudioComp->SetAutoActivate(false);
	WorldAudioComp->bAllowSpatialization = false;
	WorldAudioComp->RegisterComponentWithWorld(World);
	
	//Make an async delegate to load the sound effect
	DevSettings->CoinPickupSound.LoadAsync(
		FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &URogueCoinPickupSubsystem::OnPickupSoundLoadComplete)
		);
}

void URogueCoinPickupSubsystem::OnPickupMeshLoadComplete(const FSoftObjectPath& SoftObjectPth, UObject* LoadedObject)
{
	WorldISM->SetStaticMesh(Cast<UStaticMesh>(LoadedObject));
}

void URogueCoinPickupSubsystem::OnPickupSoundLoadComplete(const FSoftObjectPath& SoftObjectPth, UObject* LoadedObject)
{
	WorldAudioComp->SetSound(Cast<USoundBase>(LoadedObject));
}

void URogueCoinPickupSubsystem::PlayPickupSound()
{
	if(!WorldAudioComp->IsPlaying())
	{
		WorldAudioComp->Play();
	}
	WorldAudioComp->SetTriggerParameter(CoinPickupTriggerParameterName);
}

void URogueCoinPickupSubsystem::AddCoinPickups(TArray<FVector> NewLocations, TArray<int32> NewAmounts)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::AddCoinPickups);
	
	CoinLocations.Append(NewLocations);
	CoinAmounts.Append(NewAmounts);
	
	TArray<FTransform> MeshTransforms;
	for(int i = 0; i < NewLocations.Num(); ++i)
	{
		MeshTransforms.Add(FTransform(NewLocations[i] + FVector(0.0f, 0.0f, 50.0f)));
	}
	
	TArray<FPrimitiveInstanceId>NewMeshIDs = WorldISM->AddInstancesById(MeshTransforms, true, false);
	
	MeshIDs.Append(NewMeshIDs);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::RemoveCoinPickup(int32 IndexToRemove)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::RemoveCoinPickup);
	
	CoinLocations.RemoveAt(IndexToRemove);
	CoinAmounts.RemoveAt(IndexToRemove);
	
	WorldISM->RemoveInstanceById(MeshIDs[IndexToRemove]);
	MeshIDs.RemoveAt(IndexToRemove);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick);
	
	Super::Tick(DeltaTime);
	
	UWorld* World = GetWorld();
	
	FVector PlayerLocation = FVector::ZeroVector;
	for (ARoguePlayerCharacter* PlayerCharacter : TActorRange<ARoguePlayerCharacter>(World))
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}

	
	const float PickupRadius = 200.0f;
	
	// Distance Check
	TArray<int32> ProssesList;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::DistanceCheck);
		for (int i = 0; i < CoinLocations.Num(); ++i)
		{
			float Dist = FVector::Dist(PlayerLocation, CoinLocations[i]);
			if (Dist < PickupRadius)
			{
				ProssesList.Add(i);
			}
		}		
	}
	
	//Handle Pickups
	int32 TotalCoinsToGrant = 0;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::HandlePickups);
		for (int i = ProssesList.Num()-1; i >= 0; --i)
		{
			int32 CoinIndex = ProssesList[i];
			TotalCoinsToGrant += CoinAmounts[CoinIndex];
			
			RemoveCoinPickup(CoinIndex);
		}
	}
	
	if (TotalCoinsToGrant > 0)
	{
		PlayPickupSound();
	}
	
#if 0
	// @todo grant coins to the player
	UE_CLOG(TotalCoinsToGrant > 0, LogGame, Log, TEXT("Picked up coins amount: %i"), TotalCoinsToGrant);		

	
	for (int i = 0; i < CoinLocations.Num(); ++i)
	{
		DrawDebugPoint(World, CoinLocations[i], 8.0f, FColor::White);
	}
#endif
	
}

TStatId URogueCoinPickupSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(RogueCoinPickupSubsystem, STATGROUP_Tickables);
}


