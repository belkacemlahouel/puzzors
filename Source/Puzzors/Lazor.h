// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Lazor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PUZZORS_API ULazor : public UObject
{
	GENERATED_BODY()

public:
	ULazor();
	ULazor(ULazor* _parent, UParticleSystemComponent* _particle, AActor* _target);
	~ULazor();

public:
	void DestroyLazor();

// ----- MUTATOR -----
public:
	void SetParent(ULazor* _parent) { m_parent = _parent; }
	void SetChild(ULazor* _child) { m_child = _child; }

	void SetParticleSystem(UParticleSystemComponent* _particle) { m_particle = _particle; }
	void SetTarget(AActor* _target) { m_target = _target; }

	void SetPosition(FVector _position) { m_position = _position; }
	void SetDirection(FVector _direction) { m_direction = _direction; }

	UFUNCTION(BlueprintCallable, Category = "Lazor")
		bool IsChild(ULazor* _lazor);


public:
// ----- ACCESSOR -----
	UFUNCTION(BlueprintCallable, Category = "Lazor")
		ULazor* Parent() const { return m_parent; }
	
	UFUNCTION(BlueprintCallable, Category = "Lazor")
		ULazor* Child() const { return m_child; }

	UFUNCTION(BlueprintCallable, Category = "Lazor")
		AActor* Target() { return m_target; }

	const UParticleSystemComponent* ParticleSystem() const { return m_particle; }
	UParticleSystemComponent* ParticleSystem() { return m_particle; }

	UFUNCTION(BlueprintCallable, Category = "Lazor")
		FVector Position() const { return m_position; }
	UFUNCTION(BlueprintCallable, Category = "Lazor")
		FVector Direction() const { return m_direction; }

	// Count how many parent this lazor has
	int Index() const;

private:
	ULazor* m_child;
	ULazor* m_parent;

	UParticleSystemComponent* m_particle;

	AActor* m_target;

	FVector m_position;
	FVector m_direction;

};
