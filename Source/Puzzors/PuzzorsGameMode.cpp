// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "PuzzorsGameMode.h"
#include "LazorManager.h"


void APuzzorsGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APuzzorsGameMode::BeginPlay()
{
	for (TObjectIterator<ULazorManager> Itr; Itr; ++Itr)
	{
		m_LazorManager = *Itr;
	}
}