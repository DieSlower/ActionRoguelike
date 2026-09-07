// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

#include "ActionRoguelike.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/URogueDeveloperSettings.h"
#include "Player/RoguePlayerCharacter.h"

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
	
	
	const URogueDeveloperSettings* DevSettings = GetDefault<URogueDeveloperSettings>();
	
	//Get the triger name from settings
	CoinPickupTriggerParameterName = DevSettings->CoinPickupTriggerParameter;
	
	//Make an async delegate to load the static mesh
	DevSettings->CoinPickupMesh.LoadAsync(
		FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &URogueCoinPickupSubsystem::OnPickupMeshLoadComplete)
		);
	
	WorldAudioComp = NewObject<UAudioComponent>(World, NAME_None, RF_Transient);
	WorldAudioComp->SetAutoActivate(false);
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
	CoinLocations.Append(NewLocations);
	CoinAmounts.Append(NewAmounts);
	
	TArray<FTransform> MeshTransforms;
	for(int i = 0; i < NewLocations.Num(); ++i)
	{
		MeshTransforms.Add(FTransform(NewLocations[i] + FVector(0.0f, 0.0f, 50.0f)));
	}
	
	TArray<FPrimitiveInstanceId>NewMeshIDs = WorldISM->AddInstancesById(MeshTransforms, true, false);
	
	MeshIDs.Append(NewMeshIDs);
}

void URogueCoinPickupSubsystem::RemoveCoinPickup(int32 IndexToRemove)
{
	CoinLocations.RemoveAt(IndexToRemove);
	CoinAmounts.RemoveAt(IndexToRemove);
	
	WorldISM->RemoveInstanceById(MeshIDs[IndexToRemove]);
	MeshIDs.RemoveAt(IndexToRemove);
	
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UWorld* World = GetWorld();
	
	FVector PlayerLocation = FVector::ZeroVector;
	for (ARoguePlayerCharacter* PlayerCharacter : TActorRange<ARoguePlayerCharacter>(World))
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}

	
	const float PickupRadius = 200.0f;
	
	
	TArray<int32> ProssesList;
	
	for (int i = 0; i < CoinLocations.Num(); ++i)
	{
		float Dist = FVector::Dist(PlayerLocation, CoinLocations[i]);
		if (Dist < PickupRadius)
		{
			ProssesList.Add(i);
		}
	}
	
	int32 TotalCoinsToGrant = 0;
	for (int i = ProssesList.Num()-1; i >= 0; --i)
	{
		int32 CoinIndex = ProssesList[i];
		TotalCoinsToGrant += CoinAmounts[CoinIndex];
		
		RemoveCoinPickup(CoinIndex);
	}
	
	if (TotalCoinsToGrant > 0)
	{
		PlayPickupSound();
	}
	
	// @todo grant coins to the player
	UE_CLOG(TotalCoinsToGrant > 0, LogGame, Log, TEXT("Picked up coins amount: %i"), TotalCoinsToGrant);		

	
	for (int i = 0; i < CoinLocations.Num(); ++i)
	{
		DrawDebugPoint(World, CoinLocations[i], 8.0f, FColor::White);
	}
}

TStatId URogueCoinPickupSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(RogueCoinPickupSubsystem, STATGROUP_Tickables);
}


