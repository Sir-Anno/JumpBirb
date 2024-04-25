﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BirbCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JumpBirbGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


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

void ABirbCharacter::JumpBirb()
{
	// First jump, enable gravity and start game
	if (!GetCapsuleComponent()->IsGravityEnabled())
	{
		GetCapsuleComponent()->SetEnableGravity(true);

		AJumpBirbGameMode* GameMode = Cast< AJumpBirbGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->SwitchGameState(EGameState::StartGame);
	}

	if (WingFlap)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), WingFlap);
	}
	
	GetCapsuleComponent()->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f), false);
	GetCapsuleComponent()->AddImpulse(FVector(0.0f, 0.0f, JumpForce));
}

void ABirbCharacter::OnGameStateChange(EGameState GameState)
{
	switch (GameState)
	{
	case EGameState::Menu :
		ResetPlayer();
		break;
		
	case EGameState::Setup :
		ResetPlayer();
		break;

	case EGameState::StartGame :
		break;

	case EGameState::GameOver :
		break;
	}
}

void ABirbCharacter::ResetPlayer()
{
	GetCapsuleComponent()->SetEnableGravity(false);
	SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
}

