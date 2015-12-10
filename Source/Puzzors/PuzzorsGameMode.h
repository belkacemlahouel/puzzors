// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MainPlayerController.h"
#include "PuzzorsGameMode.generated.h"

class UReceiverReaction;

/**
 * 
 */
UCLASS()
class PUZZORS_API APuzzorsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	void BeginPlay();

	UFUNCTION(BlueprintCallable, Category = "Movable")
		void LockEveryMovable();

	UFUNCTION(BlueprintCallable, Category = "Get Every Receptors")
		TArray<UReceiverReaction*> GetReceptorsInScene();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		UParticleSystem* ParticleTemplate;
};
