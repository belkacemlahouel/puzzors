// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "LazorTarget.h"


// Sets default values
ALazorTarget::ALazorTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}

// Called when the game starts or when spawned
void ALazorTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALazorTarget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

