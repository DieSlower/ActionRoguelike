// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"

#include "Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

// Sets default values for this component's properties
URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URogueInteractionComponent::Interact()
{
	IRogueInteractionInterface* InteractInterface = Cast<IRogueInteractionInterface>(SelectedActor);
	if (InteractInterface != nullptr)
	{
		InteractInterface->Interact();
	}
}

// Called every frame
void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = CastChecked<APlayerController>(GetOwner());
	
	FVector Center = PC->GetPawn()->GetActorLocation();
		
	ECollisionChannel CollisionChannel = ECC_Visibility;
	
	FCollisionShape Sphere;
	Sphere.SetSphere(InteractionRadius);
	
	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Sphere);
		
	float HighestDotResult = -1.0;
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		FVector OverlapDirection = (OverlapLocation-Center).GetSafeNormal();			
		
		float DotResult = FVector::DotProduct(OverlapDirection, PC->GetControlRotation().Vector());
		if (DotResult > HighestDotResult)
		{
			SelectedActor = Overlap.GetActor();
			HighestDotResult = DotResult;
		}
		
		FString DebugString = FString::Printf(TEXT("Dot: %f"), DotResult);
		DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0, true);
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(60.f), FColor::Red);
		
	}
		
	if (SelectedActor != nullptr)
	{		
		DrawDebugBox(GetWorld(), SelectedActor->GetActorLocation(), FVector(70.f), FColor::Green);
	}
	
	DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
}

