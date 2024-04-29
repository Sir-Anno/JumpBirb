﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveSystem.generated.h"


UCLASS()
class JUMPBIRB_API USaveSystem : public USaveGame
{
	GENERATED_BODY()

public:
	USaveSystem();

	UPROPERTY(VisibleAnywhere)
	int HighScore;
};
