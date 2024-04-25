// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpBirbGameMode.h"
#include "Obstacle.h"
#include "Kismet/GameplayStatics.h"

void AJumpBirbGameMode::BeginPlay()
{
	Super::BeginPlay();

	ObstacleSpawnLocation = FVector(
		-800.0f,
		-400.0f,
		0.0f
	);

	SwitchGameState(EGameState::Menu);
}

// Used for setting up the game, showing main menu
void AJumpBirbGameMode::Setup()
{
	ResetGame();
}

// When the player starts jumping
void AJumpBirbGameMode::StartGame()
{
	SpawnObstacle();
	GetWorldTimerManager().SetTimer(SpawnObstaclesTimer, this, &AJumpBirbGameMode::SpawnObstacle, 5.0f, true);
}

// When the player collides with a wall
void AJumpBirbGameMode::GameOver()
{
	bGameOver = true;
	
	// Stop spawning new obstacles
	GetWorldTimerManager().ClearTimer(SpawnObstaclesTimer);

	if (CollisionSound)
	{
		UGameplayStatics::PlaySound2D(this, CollisionSound);
	}
}

// Reset everything
void AJumpBirbGameMode::ResetGame()
{
	bGameOver = false;
	Score = 0;

	// Clear all obstacles
	TArray<TObjectPtr<AActor>> Obstacles;
	UGameplayStatics::GetAllActorsOfClass(this, AObstacle::StaticClass(), Obstacles);
	for (int i = 0; i < Obstacles.Num(); ++i)
	{
		Obstacles[i]->Destroy();
	}
}

void AJumpBirbGameMode::SpawnObstacle()
{
	ObstacleSpawnLocation.Z = FMath::RandRange(-175.0f, 175.0f);
	const TObjectPtr<AObstacle> NewObstacle = Cast<AObstacle>(GetWorld()->SpawnActor<AActor>(Obstacle, ObstacleSpawnLocation, FRotator(0.0f, 0.0f, 0.0f)));

	NewObstacle->PlayerTriggeredScore.AddDynamic(this, &AJumpBirbGameMode::UpdateScore);
	NewObstacle->PlayerCollided.AddDynamic(this, &AJumpBirbGameMode::PlayerCollided);
}

void AJumpBirbGameMode::UpdateScore()
{
	Score ++;
	OnScoreChanged.Broadcast(Score); // Alerts UI to update

	if (ScoreSound)
	{
		UGameplayStatics::PlaySound2D(this, ScoreSound);
	}
}

void AJumpBirbGameMode::PlayerCollided()
{
	SwitchGameState(EGameState::GameOver);
}

// Switches game states and alerts other actors of change
void AJumpBirbGameMode::SwitchGameState(EGameState NewState)
{
	// If new state is the same as current don't broadcast again
	if(NewState == CurrentGameState) return;

	CurrentGameState = NewState;
	OnGameStateChange.Broadcast(CurrentGameState);
	
	switch (CurrentGameState)
	{
	case EGameState::Menu :
		ResetGame();
		break;
		
	case EGameState::Setup :
		Setup();
		break;

	case EGameState::StartGame :
		StartGame();
		break;

	case EGameState::GameOver :
		GameOver();
		break;
	}
}
