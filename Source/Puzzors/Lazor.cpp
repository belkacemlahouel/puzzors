// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Lazor.h"

ULazor::ULazor() : m_child(NULL), m_parent(NULL), m_target(NULL)
{}

ULazor::ULazor(ULazor* _parent, UParticleSystemComponent* _particle, AActor* _target)
: m_child(NULL), m_parent(_parent), m_particle(_particle), m_target(_target)
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

bool ULazor::IsChild(ULazor* _lazor)
{
	ULazor* l = Child();
	while (l != NULL)
	{
		if (l == _lazor)
			return true;
		l = l->Child();
	}
	return false;
}

void ULazor::DestroyLazor()
{
	Target()->Destroy();
	ParticleSystem()->DestroyComponent();
	SetChild(NULL);
	SetParent(NULL);

	DeleteObject(this);
}