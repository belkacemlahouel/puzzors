// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ReactOnLazorHit.h"
#include "ReceiverReaction.generated.h"

/**
 * 
 */
UCLASS()
class PUZZORS_API UReceiverReaction : public UReactOnLazorHit
{
	GENERATED_BODY()

public:
	UFUNCTION(Blueprintcallable, Category = "Receiver Reaction")
		bool IsActivated() { return Active; }

protected:
	UPROPERTY(BlueprintReadWrite)
		bool Active;
	
};
