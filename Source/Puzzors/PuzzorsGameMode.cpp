// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "PuzzorsGameMode.h"
#include "LazorManager.h"
#include "Movable.h"


void APuzzorsGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);


}

void APuzzorsGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void APuzzorsGameMode::LockEveryMovable()
{
	for (TObjectIterator<UMovable> Itr; Itr; ++Itr)
	{
		if (Itr->GetWorld() == GetWorld())
			Itr->Lock();
	}
}