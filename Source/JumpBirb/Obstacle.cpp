// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "JumpBirbGameMode.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AObstacle::AObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);
	
	TopSprite = CreateDefaultSubobject<UPaperSpriteComponent>("TopSprite");
	TopSprite->SetupAttachment(GetRootComponent());
	TopSprite->OnComponentHit.AddDynamic(this, &AObstacle::OnHit);
	
	BottomSprite = CreateDefaultSubobject<UPaperSpriteComponent>("BottomSprite");
	BottomSprite->SetupAttachment(GetRootComponent());
	BottomSprite->OnComponentHit.AddDynamic(this, &AObstacle::OnHit);

	ScoreTrigger = CreateDefaultSubobject<UBoxComponent>("ScoreTrigger");
	ScoreTrigger->SetupAttachment(GetRootComponent());
	ScoreTrigger->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::TriggerOverlap);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	TObjectPtr<AJumpBirbGameMode> GameMode = Cast<AJumpBirbGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->OnGameStateChange.AddDynamic(this, &AObstacle::OnGameStateChange);
}

void AObstacle::TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	PlayerTriggeredScore.Broadcast();
}

void AObstacle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayerCollided.Broadcast();
}

void AObstacle::OnGameStateChange(EGameState GameState)
{
	if (GameState == EGameState::GameOver)
	{
		MovementSpeed = 0.0f;
	}
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation() + FVector(MovementSpeed * DeltaTime, 0.0f, 0.0f));

	if (GetActorLocation().X >= 800.0f)
	{
		Destroy();
	}
}

