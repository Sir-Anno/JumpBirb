// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JumpBirbGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState :uint8
{
	Menu,
	Setup,
	StartGame,
	GameOver
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChange, EGameState, GameState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int, NewScore);

UCLASS()
class JUMPBIRB_API AJumpBirbGameMode : public AGameModeBase
{
	GENERATED_BODY()

	FVector ObstacleSpawnLocation;

	bool bGameOver = false;
	
	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChanged;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TSubclassOf<AActor> Obstacle;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> ScoreSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> CollisionSound;

	FTimerHandle SpawnObstaclesTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Setup();
	void StartGame();
	void GameOver();
	void ResetGame();

	void SpawnObstacle();

	UFUNCTION()
	void UpdateScore();

	UFUNCTION()
	void PlayerCollided();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentGameState;
	
	UPROPERTY(BlueprintAssignable)
	FOnGameStateChange OnGameStateChange;
	
	UFUNCTION(BlueprintCallable)
	void SwitchGameState(EGameState NewState);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Score = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int HighScore = 0;
};
