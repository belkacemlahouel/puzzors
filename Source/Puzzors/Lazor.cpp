// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Lazor.h"

Lazor::Lazor(Lazor* _parent, UParticleSystemComponent* _particle, const AActor* _source, const AActor* _target)
: m_child(NULL), m_parent(_parent), m_particle(_particle), m_source(_source), m_target(_target)
{
}

Lazor::~Lazor()
{
}


int Lazor::Index() const
{
	int i = 0;
	const Lazor* l = Parent();
	while (l != NULL)
	{
		l = l->Parent();
		++i;
	}
	return i;
}