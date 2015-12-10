// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "InteractiveActor.h"


// Sets default values for this component's properties
UInteractiveActor::UInteractiveActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractiveActor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractiveActor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

