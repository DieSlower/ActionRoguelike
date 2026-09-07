// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "URogueDeveloperSettings.generated.h"

class UStaticMesh;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class ACTIONROGUELIKE_API URogueDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Config, EditDefaultsOnly, Category=Pickup)
	TSoftObjectPtr<UStaticMesh> CoinPickupMesh;
	
	UPROPERTY(Config, EditDefaultsOnly, Category=Pickup)
	TSoftObjectPtr<USoundBase> CoinPickupSound;
	
	UPROPERTY(Config, EditDefaultsOnly, Category=Pickup)
	FName CoinPickupTriggerParameter;
	
	virtual FName GetCategoryName() const override;
	
};
