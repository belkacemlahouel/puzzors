// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Beam.generated.h"

class UReactOnLazorHit;
class ULazorManager;
class ULazor;

USTRUCT()
struct FLazorHit
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		UReactOnLazorHit* reactor;
	
	UPROPERTY()
		ULazor* lazor;

	UPROPERTY()
		FHitResult hit;

	FLazorHit(UReactOnLazorHit* _reactor, ULazor* _lazor, FHitResult _hit)
	{
		reactor = _reactor; lazor = _lazor; hit = _hit;
	}

	FLazorHit()
	{
		reactor = NULL; lazor = NULL;
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class PUZZORS_API UBeam : public UObject
{
	GENERATED_BODY()

public:
	UBeam();
	UBeam(const FVector& _position, const FVector& _direction, ULazorManager* _manager);
	virtual ~UBeam();

	UFUNCTION(BlueprintCallable, Category = "Beam")
		void FireBeam();
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void ClearBeam(bool _callHitEnd);

	bool HasHit(UReactOnLazorHit* _reactor);

	bool Initialized() { return m_manager != NULL; }

	UWorld* GetWorld() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void SetPosition(const FVector& _position) { m_position = _position; }
	UFUNCTION(BlueprintCallable, Category = "Beam")
		void SetDirection(const FVector& _direction) { m_direction = _direction; }
	
	void SetManager(ULazorManager* _manager) { m_manager = _manager; }

private:
	ULazor* m_root;

	FVector m_position;
	FVector m_direction;

	TArray<FLazorHit> m_hit;

	ULazorManager* m_manager;

private:
	UParticleSystemComponent* InstantiateParticleSystem(const FVector& _location, const FRotator& _direction);
	ULazor* InstanciateLazor(UParticleSystemComponent* _particle, ULazor* _parent);

	AActor* ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult);
	UReactOnLazorHit* ExtractReactorFromActor(AActor* _actor);
};
