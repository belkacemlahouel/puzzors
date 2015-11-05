// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Lazor.generated.h"

/**
 * 
 */
UCLASS()
class PUZZORS_API ULazor : public UObject
{
	GENERATED_BODY()

public:
	ULazor();
	ULazor(ULazor* _parent, UParticleSystemComponent* _particle, const AActor* _source, AActor* _target);
	~ULazor();

// ----- MUTATOR -----
public:
	void SetParent(ULazor* _parent) { m_parent = _parent; }
	void SetChild(ULazor* _child) { m_child = _child; }

	void SetParticleSystem(UParticleSystemComponent* _particle) { m_particle = _particle; }
	
	void SetSource(const AActor* _source) { m_source = _source; }
	void SetTarget(AActor* _target) { m_target = _target; }

	void SetPosition(FVector _position) { m_position = _position; }
	void SetDirection(FVector _direction) { m_direction = _direction; }


public:
// ----- ACCESSOR -----
	ULazor* Parent() const { return m_parent; }
	ULazor* Child() const { return m_child; }

	const AActor* Source() const { return m_source; }
	AActor* Target() { return m_target; }

	const UParticleSystemComponent* ParticleSystem() const { return m_particle; }
	UParticleSystemComponent* ParticleSystem() { return m_particle; }

	const FVector& Position() { return m_position; }
	const FVector& Direction() { return m_direction; }

	// Count how many parent this lazor has
	int Index() const;

private:
	ULazor* m_child;
	ULazor* m_parent;

	UParticleSystemComponent* m_particle;

	const AActor* m_source;
	AActor* m_target;

	FVector m_position;
	FVector m_direction;

};
