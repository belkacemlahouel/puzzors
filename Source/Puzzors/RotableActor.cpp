// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "RotableActor.h"


// Sets default values for this component's properties
URotableActor::URotableActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotableActor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotableActor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void URotableActor::Rotate()
{
	FRotator rot = GetOwner()->GetActorRotation();
	rot.Add(0, RotateStep, 0);
	this->GetOwner()->SetActorRotation(rot);
}