// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Movable.h"

#include "MoveEventHandler.h"


// Sets default values for this component's properties
UMovable::UMovable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	m_lock = false;
	// ...
}


// Called when the game starts
void UMovable::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMovable::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UMovable::Move(const FRotator& _RotationDeltas, const FVector& _TranslationDeltas)
{
	if (!m_lock && !(_RotationDeltas.IsNearlyZero() && _TranslationDeltas.IsNearlyZero()))
	{
		Rotate(_RotationDeltas);
		Translate(_TranslationDeltas);

		SendMoveEvent();
	}
}

void UMovable::SendMoveEvent()
{
	OnMove.Broadcast();
}

void UMovable::Rotate_Implementation(const FRotator& _Deltas)
{
	FRotator rot = GetOwner()->GetActorRotation();
	rot.Add(_Deltas.Pitch, _Deltas.Yaw, _Deltas.Roll);
	GetOwner()->SetActorRotation(rot);
}

void UMovable::Translate_Implementation(const FVector& _Deltas)
{
	FVector loc = GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(loc + _Deltas);
}