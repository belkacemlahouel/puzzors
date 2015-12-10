// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Beam.h"
#include "LazorManager.h"
#include "LazorTarget.h"
#include "ReactOnLazorHit.h"
#include "PuzzorsGameMode.h"

ABeam::ABeam() : m_direction(1, 0, 0), ParticleTemplate(nullptr), m_root(nullptr), m_color(EBeamColor::BC_RED)
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

ABeam::ABeam(const FVector& _direction) : m_direction(_direction), ParticleTemplate(nullptr), m_root(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

ABeam::~ABeam()
{}


void ABeam::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABeam::BeginPlay()
{
	Super::BeginPlay();

	APuzzorsGameMode* mode = Cast<APuzzorsGameMode>(GetWorld()->GetAuthGameMode());
	if (mode != nullptr)
		ParticleTemplate = mode->ParticleTemplate;
}

void ABeam::BeginDestroy()
{
	Super::BeginDestroy();
	if (m_TimerHandle.IsValid() && GetWorld() != nullptr)
		GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	ClearBeam(false);
}

void ABeam::Activate()
{
	FireBeam();
	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &ABeam::FireBeam, 0.10f, true);
}

void ABeam::Deactivate()
{
	GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	ClearBeam(true);
}

void ABeam::FireBeam()
{
	if (!Initialized())
		return;

	TArray<FLazorHit> oldHit(m_hit);
	ClearBeam(false);

	bool loop;

	FVector pos = GetActorLocation(), dir = m_direction;
	FLazor* lazor = nullptr;
	do
	{
		loop = false;

		// instanciating lazor
		UParticleSystemComponent* particle = InstantiateParticleSystem(pos, FRotator::ZeroRotator);
		lazor = InstanciateLazor(particle, lazor);
		lazor->SetPosition(pos);
		lazor->SetDirection(dir);
		if (m_root == nullptr) m_root = lazor;

		// computing target
		FHitResult hit;
		AActor* target = ComputeLazorTarget(pos, dir, hit);
		bool hasHit = hit.IsValidBlockingHit();
		lazor->SetTarget(target);
		particle->SetActorParameter("Target", target);
		particle->SetColorParameter("Color", BeamColorToLinearColor(m_color));

		// what have we hit
		if (hasHit)
		{
			UReactOnLazorHit* reactor = ExtractReactorFromActor(hit.GetActor());
			if (reactor != nullptr)
			{
				m_hit.Add(FLazorHit(reactor, lazor, hit));
				reactor->OnLazorHit(hit.ImpactPoint, dir, hit.Component.Get(), this);
				if (lazor->Index() < 10 && reactor->IsBouncingLazor(hit.ImpactPoint, hit.Component.Get()))
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
			h.Reactor()->OnLazorHitBegin(h.hit.ImpactPoint, h.Lazor()->Direction(), h.hit.Component.Get(), this);
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

bool ABeam::HasHit(UReactOnLazorHit* _reactor)
{
	for (auto lh : m_hit)
	{
		if (lh.Reactor() == _reactor)
			return true;
	}

	return false;
}

void ABeam::ClearBeam(bool _callHitEnd)
{
	if (_callHitEnd)
	{
		for (auto lh : m_hit)
			lh.Reactor()->OnLazorHitEnd(this);
	}

	FLazor* l = m_root;
	FLazor* temp = nullptr;
	while (l != nullptr)
	{
		temp = l;
		l = l->Child();
		
		temp->DestroyLazor();
		delete temp;
	}
	m_root = nullptr;

	m_hit.Empty();
}

UReactOnLazorHit* ABeam::ExtractReactorFromActor(AActor* _actor)
{
	if (_actor == nullptr) return nullptr;

	UReactOnLazorHit* reactor = nullptr;

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

UParticleSystemComponent* ABeam::InstantiateParticleSystem(const FVector& _location, const FRotator& _direction)
{
	UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAttached(ParticleTemplate, RootComponent, NAME_None, _location, _direction, EAttachLocation::KeepWorldPosition);
	FParticleSysParam param;
	param.Name = "Target";
	param.ParamType = EParticleSysParamType::PSPT_Actor;

	FParticleSysParam colorParam;
	colorParam.Name = "Color";
	colorParam.ParamType = EParticleSysParamType::PSPT_Color;

	particleSystem->InstanceParameters.Add(param);
	particleSystem->InstanceParameters.Add(colorParam);
	particleSystem->UpdateInstances(true);
	particleSystem->ActivateSystem();

	return particleSystem;
}

AActor* ABeam::ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult)
{
	// Create the target
	AActor* target = GetWorld()->SpawnActor(ALazorTarget::StaticClass());

	// Search for the target's position
	FVector dir = _inDir;
	FVector end = _inPos + dir * 100000;
	bool hasHit = GetWorld()->LineTraceSingleByChannel(_outHitResult, _inPos, end, ECC_PhysicsBody);
	if (hasHit)
		target->SetActorLocation(_outHitResult.ImpactPoint);
	else
		target->SetActorLocation(end);

	return target;
}

FLinearColor ABeam::BeamColorToLinearColor(EBeamColor _color)
{
	switch (_color)
	{
	case EBeamColor::BC_RED: return BEAM_RED_COLOR;
	case EBeamColor::BC_GREEN: return BEAM_GREEN_COLOR;
	case EBeamColor::BC_BLUE: return BEAM_BLUE_COLOR;
	}

	return BEAM_DEFAULT_COLOR;
}