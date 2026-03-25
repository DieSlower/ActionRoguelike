// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RogueAttributeSet.generated.h"


class URogueActionSystemComponent;

USTRUCT()
struct FRogueAttribute
{
	GENERATED_BODY()
	
	FRogueAttribute() {}
	
	FRogueAttribute(float InBase)
		: Base(InBase) {}
	
	UPROPERTY(EditAnywhere)
	float Base = 0.f;
	
	UPROPERTY(Transient)
	float Modifier = 0.f;
	
	float GetValue() {return Base + Modifier;}
		
};


/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueAttributeSet : public UObject
{
	GENERATED_BODY()
public:
	virtual void PostAttributeChanged() {};
	
	virtual void InitializeAttributes() {};
	
protected:
	
	URogueActionSystemComponent* GetOwningComponent() const;
	
};

UCLASS()
class URogueHealthAttributeSet : public URogueAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	FRogueAttribute Health;
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	FRogueAttribute HealthMax;

	URogueHealthAttributeSet();
	
	virtual void PostAttributeChanged() override;
		
};


UCLASS()
class URoguePawnAttributeSet : public URogueHealthAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	FRogueAttribute MoveSpeed;
	
	URoguePawnAttributeSet();
	
	virtual void PostAttributeChanged() override;
	
	virtual void InitializeAttributes() override;
	
	void ApplyMoveSpeed();
};

UCLASS()
class URoguePlayerAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	FRogueAttribute RageAmount;
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	FRogueAttribute RageAmountMax;

	
	URoguePlayerAttributeSet();
	
	virtual void PostAttributeChanged() override;
	
	virtual void InitializeAttributes() override;
	
	void ApplyRageAmount();
	
public:
};

UCLASS()
class URogueMonsterAttributeSet : public URoguePawnAttributeSet
{
	GENERATED_BODY()
	
public:
	
	URogueMonsterAttributeSet();
};