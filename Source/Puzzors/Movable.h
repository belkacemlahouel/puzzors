// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Movable.generated.h"

class IMoveEventHandler;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZORS_API UMovable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMovable();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void Move(const FRotator& _RotationDeltas, const FVector& _TranslationDeltas);

	void AddEventHandler(IMoveEventHandler* _handler);
	void RemoveEventHandler(IMoveEventHandler* _handler);

protected:
	virtual void Rotate(const FRotator& _Deltas);
	virtual void Translate(const FVector& _Deltas);

private:
	void SendMoveEvent();
	TArray<IMoveEventHandler*> m_MoveEventHandlers;

};
