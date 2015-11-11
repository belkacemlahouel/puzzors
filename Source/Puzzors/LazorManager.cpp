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
		DeleteObject(var);
	}
	Beams.Empty();
}

UBeam* ULazorManager::CreateBeam(const FVector& _inPos, const FVector& _inDir)
{
	UBeam* beam = NewObject<UBeam>();
	beam->SetManager(this);
	beam->SetPosition(_inPos);
	beam->SetDirection(_inDir);

	beam->FireBeam();

	Beams.Add(beam);

	return beam;
}


void ULazorManager::UpdateBeam(UBeam* _beam)
{

}
