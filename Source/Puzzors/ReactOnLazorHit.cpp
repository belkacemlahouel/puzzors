// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "ReactOnLazorHit.h"

UReactOnLazorHit::UReactOnLazorHit()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UReactOnLazorHit::IsBouncingLazor_Implementation(ULazor* _lazor, const FVector& _hitPoint)
{
	return true;
}

FVector UReactOnLazorHit::BounceLazor_Implementation(const FVector& _dir)
{
	return FVector::UpVector;
}