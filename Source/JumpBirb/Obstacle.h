// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JumpBirbGameMode.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerTriggeredScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerCollided);

UCLASS()
class JUMPBIRB_API AObstacle : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> TopSprite;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> BottomSprite;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> ScoreTrigger;

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 100.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnGameStateChange(EGameState GameState);

public:
	// Sets default values for this actor's properties
	AObstacle();

	UPROPERTY()
	FPlayerTriggeredScore PlayerTriggeredScore;

	UPROPERTY()
	FPlayerCollided PlayerCollided;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
