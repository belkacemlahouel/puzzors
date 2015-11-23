// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Beam.h"
#include "LazorManager.h"
#include "LazorTarget.h"
#include "ReactOnLazorHit.h"

UBeam::UBeam() : m_position(), m_direction(1, 0, 0), m_manager(NULL), m_root(NULL)
{}

UBeam::UBeam(const FVector& _position, const FVector& _direction, ULazorManager* _manager) : m_position(_position), m_direction(_direction), m_manager(_manager), m_root(NULL)
{}

UBeam::~UBeam()
{}

void UBeam::Activate()
{
	FireBeam();
	m_manager->GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &UBeam::FireBeam, 0.10f, true);
}

void UBeam::Deactivate()
{
	m_manager->GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	ClearBeam(true);
}

void UBeam::FireBeam()
{
	if (!Initialized())
		return;

	TArray<FLazorHit> oldHit(m_hit);
	ClearBeam(false);

	bool loop;

	FVector pos = m_position, dir = m_direction;
	FLazor* lazor = NULL;
	do
	{
		loop = false;

		// instanciating lazor
		UParticleSystemComponent* particle = InstantiateParticleSystem(pos, FRotator::ZeroRotator);
		lazor = InstanciateLazor(particle, lazor);
		lazor->SetPosition(pos);
		lazor->SetDirection(dir);
		if (m_root == NULL) m_root = lazor;

		// computing target
		FHitResult hit;
		AActor* target = ComputeLazorTarget(pos, dir, hit);
		bool hasHit = hit.IsValidBlockingHit();
		lazor->SetTarget(target);
		particle->SetActorParameter("Target", target);

		// what have we hit
		if (hasHit)
		{
			UReactOnLazorHit* reactor = ExtractReactorFromActor(hit.GetActor());
			if (reactor != NULL)
			{
				m_hit.Add(FLazorHit(reactor, lazor, hit));
				reactor->OnLazorHit(hit.ImpactPoint, dir, this);
				if (lazor->Index() < 10 && reactor->IsBouncingLazor(hit.ImpactPoint))
				{
					pos = hit.ImpactPoint;
					dir = reactor->BounceLazor(dir);
					loop = true;
				}
			}

		}
	} while (loop);

	// Comparing old hit array and new hit array to see if we got events to fire
	
	// checking for BeginHit events
	for (auto h : m_hit)
	{
		bool found = false;
		for (auto oh : oldHit)
		{
			if (h.reactor == oh.reactor) // was already hit
			{
				found = true;
				break;
			}
		}

		if (!found) // newly hit
		{
			h.Reactor()->OnLazorHitBegin(h.hit.ImpactPoint, h.Lazor()->Direction(), this);
		}
	}

	// checking for EndHit events
	for (auto oh : oldHit)
	{
		bool found = false;
		for (auto h : m_hit)
		{
			if (oh.reactor == h.reactor) // was already
			{
				found = true;
				break;
			}
		}

		if (!found) // was hit but now is not
		{
			oh.Reactor()->OnLazorHitEnd(this);
		}
	}
}

bool UBeam::HasHit(UReactOnLazorHit* _reactor)
{
	for (auto lh : m_hit)
	{
		if (lh.Reactor() == _reactor)
			return true;
	}

	return false;
}

void UBeam::ClearBeam(bool _callHitEnd)
{
	if (_callHitEnd)
	{
		for (auto lh : m_hit)
			lh.Reactor()->OnLazorHitEnd(this);
	}

	FLazor* l = m_root;
	FLazor* temp = NULL;
	while (l != NULL)
	{
		temp = l;
		l = l->Child();
		
		temp->DestroyLazor();
		delete temp;
	}
	m_root = NULL;

	m_hit.Empty();
}

UReactOnLazorHit* UBeam::ExtractReactorFromActor(AActor* _actor)
{
	if (_actor == NULL) return NULL;

	UReactOnLazorHit* reactor = NULL;

	TArray<UActorComponent*> components = _actor->GetComponents();
	for (UActorComponent* comp : components)
	{
		if (comp->GetClass()->IsChildOf<UReactOnLazorHit>())
		{
			// found a reactor !
			reactor = CastChecked<UReactOnLazorHit>(comp);
			break;
		}
	}

	return reactor;
}

UParticleSystemComponent* UBeam::InstantiateParticleSystem(const FVector& _location, const FRotator& _direction)
{
	UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAttached(m_manager->ParticleTemplate, m_manager, NAME_None, _location, _direction);
	FParticleSysParam param;
	param.Name = "Target";
	param.ParamType = EParticleSysParamType::PSPT_Actor;
	particleSystem->InstanceParameters.Add(param);
	particleSystem->UpdateInstances(true);
	particleSystem->ActivateSystem();

	return particleSystem;
}
/*
FLazor* UBeam::InstanciateLazor(UParticleSystemComponent* _particle, FLazor* _parent)
{
	FLazor* lazor = new FLazor();
	lazor->SetParticleSystem(_particle);
	lazor->SetParent(_parent);
	if (_parent != NULL) _parent->SetChild(lazor);
	return lazor;
}*/

AActor* UBeam::ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult)
{
	// Create the target
	AActor* target = GetWorld()->SpawnActor(ALazorTarget::StaticClass());

	// Search for the target's position
	FVector dir = _inDir;
	FVector end = _inPos + dir * 100000; // Relative is used because the LazorManager is centered (pos={0,0,0})
	bool hasHit = GetWorld()->LineTraceSingleByChannel(_outHitResult, _inPos, end, ECC_PhysicsBody);
	if (hasHit)
		target->SetActorLocation(_outHitResult.ImpactPoint);
	else
		target->SetActorLocation(end);

	return target;
}

UWorld* UBeam::GetWorld() const
{
	if (m_manager == NULL) return NULL; 
	return m_manager->GetWorld();
}