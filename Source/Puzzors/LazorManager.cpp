// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "LazorManager.h"
#include "Beam.h"


// Sets default values for this component's properties
ULazorManager::ULazorManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void ULazorManager::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto var : Beams)
	{
		var->ClearBeam(false);
		DeleteObject(var);
	}
	Beams.Empty();
}

ABeam* ULazorManager::CreateBeam(const FVector& _inPos, const FVector& _inDir)
{
	return nullptr;
}