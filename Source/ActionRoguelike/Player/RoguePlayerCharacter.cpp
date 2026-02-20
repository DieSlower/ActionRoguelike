// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "Projectiles/RogueProjectileMagic.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

TAutoConsoleVariable<float> CVarProjectileAdjustmentDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.f, TEXT("Enable projectile aim debug drawing. (0 = disabled, >0 = time to show on screen"), ECVF_Cheat);

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp")); 
	
	MuzzleSocketName = "Muzzle_01";
	Muzzle2SocketName = "Muzzle_02";
	MuzzleTeleportSocketName = MuzzleSocketName;
}


void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ARoguePlayerCharacter::OnHealthChanged);
}

// Called to bind functionality to input
void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
	
	EnhancedInput->BindAction(Input_JumpAction, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Jump);
	
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartAction, FName("PrimaryAttack"));
	
	
	EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::SecondaryAttack);
	EnhancedInput->BindAction(Input_TeleportAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::SpecialAttack);
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0;
	
	// Forward/Back
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	// Side to side strafe
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	// Died?
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		
		GetMovementComponent()->StopMovementImmediately();
		
		PlayAnimMontage(DeathMontoge);
	}
}

void ARoguePlayerCharacter::StartAction(FName InActionName)
{
	ActionSystemComponent->StartAction(InActionName);
}

void ARoguePlayerCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackMontoge);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true );
	
	UGameplayStatics::PlaySound2D(this, CastingSound);

	FTimerHandle AttackTimerHandle;
	const float AttackDelayTime = 0.2f;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ARoguePlayerCharacter::AttackTimerElapsed, AttackDelayTime);
	
}

void ARoguePlayerCharacter::SecondaryAttack()
{
	PlayAnimMontage(Attack2Montoge);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(Casting2Effect, GetMesh(), Muzzle2SocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true );
	
	UGameplayStatics::PlaySound2D(this, Casting2Sound);
	
	FTimerHandle Attack2TimerHandle;
	const float Attack2DelayTime = 0.2f;
	GetWorldTimerManager().SetTimer(Attack2TimerHandle, this, &ARoguePlayerCharacter::Attack2TimerElapsed, Attack2DelayTime);
}

void ARoguePlayerCharacter::SpecialAttack()
{
	PlayAnimMontage(AttackTeleportMontoge);
	
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingTeleportEffect, GetMesh(), MuzzleTeleportSocketName, 
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true );
	
	UGameplayStatics::PlaySound2D(this, CastingTeleportSound);
	
	FTimerHandle AttackTeleportTimerHandle;
	const float AttackTeleportDelayTime = 0.2f;
	GetWorldTimerManager().SetTimer(AttackTeleportTimerHandle, this, &ARoguePlayerCharacter::AttackTeleportTimerElapsed, AttackTeleportDelayTime);
}

void ARoguePlayerCharacter::AttackTimerElapsed()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	FVector EyeLocation = CameraComponent->GetComponentLocation();
	FRotator EyeRotation = GetControlRotation();
	
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 15000.f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	UWorld* World = GetWorld();
	
	FVector AdjustTargetLocation;
	
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE))
	{
		AdjustTargetLocation = Hit.Location;
	}
	else
	{
		AdjustTargetLocation = TraceEnd;
	}
	FRotator SpawnRotation = (AdjustTargetLocation - SpawnLocation).Rotation();
    AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	MoveIgnoreActorAdd(NewProjectile);
	
#if !UE_BUILD_SHIPPING
	float DebugDrawDuration = CVarProjectileAdjustmentDebugDrawing.GetValueOnGameThread();
	
	if (DebugDrawDuration > 0.0f)
	{
		// Hit location of Trace End
		DrawDebugBox(World, AdjustTargetLocation, FVector(20), FColor::Green, false, DebugDrawDuration);
		// Adjustment line trace
		DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
		// New projectile path
		DrawDebugLine(World, SpawnLocation, AdjustTargetLocation, FColor::Yellow, false, DebugDrawDuration);
		//Original projectile path
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (GetControlRotation().Vector() * 15000), FColor::Purple, false, DebugDrawDuration);	
	}	
#endif
}

void ARoguePlayerCharacter::Attack2TimerElapsed()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(Muzzle2SocketName);
	FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(Projectile2Class, SpawnLocation, SpawnRotation, SpawnParams);
	MoveIgnoreActorAdd(NewProjectile);
}

void ARoguePlayerCharacter::AttackTeleportTimerElapsed()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleTeleportSocketName);
	FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileTeleportClass, SpawnLocation, SpawnRotation, SpawnParams);
	MoveIgnoreActorAdd(NewProjectile);
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	
	ActionSystemComponent->ApplyHealthChange(-ActualDamage);
	
	return ActualDamage;
}





