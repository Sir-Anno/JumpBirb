// Fill out your copyright notice in the Description page of Project Settings.

#include "BirbCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JumpBirbGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABirbCharacter::ABirbCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ABirbCharacter::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AJumpBirbGameMode> GameMode = Cast<AJumpBirbGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnGameStateChange.AddDynamic(this, &ABirbCharacter::OnGameStateChange);

	DisableInput(GetWorld()->GetFirstPlayerController());

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

// Called every frame
void ABirbCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotates actor up when jumping and down when falling
	const FVector2D InRange = FVector2d(-1, 1);
	const FVector2D OutRange = FVector2d(0, 1);
	const FRotator UpRotation = FRotator(-20.0f, 0.0f, 0.0f);
	const FRotator DownRotation = FRotator(20.0f, 0.0f, 0.0f);
	const float Velocity = GetCapsuleComponent()->GetPhysicsLinearVelocity().Z;
	const float Alpha = FMath::GetMappedRangeValueClamped(InRange, OutRange, Velocity);
	const FRotator NewRotation = UKismetMathLibrary::RLerp(DownRotation, UpRotation, Alpha, false);
	SetActorRotation(NewRotation);
}

// Called to bind functionality to input
void ABirbCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABirbCharacter::JumpBirb);
	}
}

// Jump
void ABirbCharacter::JumpBirb()
{
	// First jump, enable gravity and start game
	if (!GetCapsuleComponent()->IsGravityEnabled())
	{
		GetCapsuleComponent()->SetEnableGravity(true);

		AJumpBirbGameMode* GameMode = Cast<AJumpBirbGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->SwitchGameState(EGameState::StartGame);
	}

	if (WingFlap)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), WingFlap);
	}
	
	GetCapsuleComponent()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false);
	GetCapsuleComponent()->AddImpulse(FVector(0.0f, 0.0f, JumpForce));
	GetCapsuleComponent()->AddImpulse(FVector(0.0f, 0.0f, -10));
}

void ABirbCharacter::OnGameStateChange(EGameState GameState)
{
	switch (GameState)
	{
	case EGameState::Menu :
		ResetPlayer();
		break;
		
	case EGameState::Setup :
		EnableInput(GetWorld()->GetFirstPlayerController());
		ResetPlayer();
		break;

	case EGameState::StartGame :
		break;

	case EGameState::GameOver :
		OnGameOver();
		break;
	}
}

void ABirbCharacter::OnGameOver()
{
	DisableInput(GetWorld()->GetFirstPlayerController());
	GetCapsuleComponent()->SetEnableGravity(false);
	GetCapsuleComponent()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false);
}

// Reset
void ABirbCharacter::ResetPlayer()
{
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
}

