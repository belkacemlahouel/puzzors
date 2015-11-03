// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class PUZZORS_API Lazor
{
public:
	Lazor(Lazor* _parent, UParticleSystemComponent* _particle, const AActor* _source, const AActor* _target);
	~Lazor();

// ----- MUTATOR -----
public:
	void SetParent(Lazor* _parent) { m_parent = _parent; }
	void SetChild(Lazor* _child) { m_child = _child; }
	
	void SetSource(AActor* _source) { m_source = _source; }
	void SetTarget(AActor* _target) { m_target = _target; }


public:
// ----- ACCESSOR -----
	const Lazor* Parent() const { return m_parent; }
	const Lazor* Child() const { return m_child; }

	const AActor* Source() const { return m_source; }
	const AActor* Target() const { return m_target; }

	const UParticleSystemComponent* ParticleSystem() const { return m_particle; }

	// Count how many parent this lazor has
	int Index() const;

private:
	Lazor* m_child;
	Lazor* m_parent;

	UParticleSystemComponent* m_particle;

	const AActor* m_source;
	const AActor* m_target;

};
