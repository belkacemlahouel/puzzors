// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ReactOnLazorHit.generated.h"

class ULazor;
class UBeam;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZORS_API UReactOnLazorHit : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReactOnLazorHit();

	UFUNCTION(BlueprintImplementableEvent)
		virtual void OnLazorHit(const FVector& _hitPos, const FVector& _hitDir, UBeam* _beam);

	UFUNCTION(BlueprintImplementableEvent)
		virtual void OnLazorHitBegin(const FVector& _hitPos, const FVector& _hitDir, UBeam* _beam);

	UFUNCTION(BlueprintImplementableEvent)
		virtual void OnLazorHitEnd(UBeam* _beam);

	UFUNCTION(BlueprintNativeEvent, Category = "Lazor Reaction")
		bool IsBouncingLazor(ULazor* _lazor, const FVector& _hitPoint);

	UFUNCTION(BlueprintNativeEvent, Category = "Lazor Reaction")
		FVector BounceLazor(const FVector& _dir);

	
};
