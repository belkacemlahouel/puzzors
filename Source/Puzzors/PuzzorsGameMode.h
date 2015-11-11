// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MainPlayerController.h"
#include "PuzzorsGameMode.generated.h"

class ULazorManager;

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

	UFUNCTION(BlueprintCallable, Category="Lazor Manager")
		ULazorManager* LazorManager() { return m_LazorManager; }
	
private:
	ULazorManager* m_LazorManager;
};
