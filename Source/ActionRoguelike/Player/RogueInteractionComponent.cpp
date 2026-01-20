// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "RogueGameTypes.h"
#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable<bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, TEXT("Enable Interaction components debug drawing.0 disable, 1 enable"), ECVF_Cheat);

// Sets default values for this component's properties
URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URogueInteractionComponent::Interact()
{
	if (SelectedActor != nullptr)
	{
		IRogueInteractionInterface::Execute_Interact(SelectedActor);
	}
}

// Called every frame
void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	
	FVector Center = PC->GetPawn()->GetActorLocation();
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	
	FCollisionShape Sphere;
	Sphere.SetSphere(InteractionRadius);
	
	float InteractionRadiusSqrd = InteractionRadius * InteractionRadius;
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Sphere);
		
	float HighestWeight = 0.0;
	
	bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		FVector Origin;
		FVector BoxExtends; 
		
		Overlap.GetActor()->GetActorBounds(true, Origin, BoxExtends  );
		
		//FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		FVector OverlapDirection = (Origin-CameraLocation).GetSafeNormal();			
		
		float DistanceToSqrd = (Origin - Center).SizeSquared();
		float NormalizedDistanceTo = 1.0f - (DistanceToSqrd / InteractionRadiusSqrd);
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		float NormalizedDotResult = DotResult * 0.5f + 0.5f;  
		
		float Weight = NormalizedDotResult * DirectionWeightScale + NormalizedDistanceTo * DistanceToWeightScale;
		if (Weight > HighestWeight)
		{
			SelectedActor = Overlap.GetActor();
			HighestWeight = DotResult;
		}
		
		if (bEnabledDebugDraw)
		{
			FString DebugString = FString::Printf(TEXT("Weight %f, Dot: %f, Dist: %f"), Weight, NormalizedDotResult, NormalizedDistanceTo);
			DrawDebugString(GetWorld(), Origin, DebugString, nullptr, FColor::White, 0, true);
			DrawDebugBox(GetWorld(), Origin, FVector(60.f), FColor::Red);	
		}		
	}
		
	if (bEnabledDebugDraw)
	{
		if (SelectedActor != nullptr)
		{		
			DrawDebugBox(GetWorld(), SelectedActor->GetActorLocation(), FVector(70.f), FColor::Green);
		}
	
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);	
	}	
}

