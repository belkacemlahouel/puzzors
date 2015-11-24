// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Movable.generated.h"

class IMoveEventHandler;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoveDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UPROPERTY(BlueprintAssignable, Category = "Movable")
		FMoveDelegate OnMove;

	UFUNCTION(BlueprintCallable, Category = "Movable")
		bool IsLocked() { return m_lock; }

	UFUNCTION(BlueprintCallable, Category = "Movable")
		void Lock() { m_lock = true; }

	UFUNCTION(BlueprintCallable, Category = "Movable")
		void UnLock() { m_lock = false; }

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Movable")
		void Rotate(const FRotator& _Deltas);

	UFUNCTION(BlueprintNativeEvent, Category = "Movable")
		void Translate(const FVector& _Deltas);

private:
	void SendMoveEvent();

	UPROPERTY(EditAnywhere)
		bool m_lock;
};
