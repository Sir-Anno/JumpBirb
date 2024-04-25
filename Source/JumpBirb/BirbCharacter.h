// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpBirbGameMode.h"
#include "PaperCharacter.h"
#include "BirbCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class JUMPBIRB_API ABirbCharacter : public APaperCharacter
{
	GENERATED_BODY()

	// Input mapping context
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> MappingContext;

	// Jump Input Action
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	// Jump force
	UPROPERTY(EditAnywhere)
	float JumpForce = 0;

	// Wingflap sound
	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> WingFlap;

protected:
	// Sets default values for this character's properties
	ABirbCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Jump action
	void JumpBirb();

	UFUNCTION()
	void OnGameStateChange(EGameState GameState);

	void ResetPlayer();
};
