// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Lazor.h"

ULazor::ULazor() : m_child(NULL), m_parent(NULL), m_source(NULL), m_target(NULL)
{}

ULazor::ULazor(ULazor* _parent, UParticleSystemComponent* _particle, const AActor* _source, AActor* _target)
: m_child(NULL), m_parent(_parent), m_particle(_particle), m_source(_source), m_target(_target)
{
}

ULazor::~ULazor()
{
}


int ULazor::Index() const
{
	int i = 0;
	const ULazor* l = Parent();
	while (l != NULL)
	{
		l = l->Parent();
		++i;
	}
	return i;
}