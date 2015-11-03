// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "LazorManager.h"
#include "LazorTarget.h"
#include "Mirror.h"
#include "Lazor.h"


// Sets default values for this component's properties
ULazorManager::ULazorManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULazorManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void ULazorManager::BeginDestroy()
{
	Super::BeginDestroy();

	for (auto var : Lazors)
	{
		delete var.Value;
	}
	Lazors.Empty();
}


// Called every frame
void ULazorManager::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

Lazor* ULazorManager::CreateLazor(const FVector& _inPos, const FVector& _inDir, const AActor* _source)
{
	// Create Particle system
	UParticleSystemComponent* particleSystem = InstantiateParticleSystem(_source->GetActorLocation(), _source->GetActorRotation());

	// Create the lazor
	Lazor* lazor = new Lazor(NULL, particleSystem, _source, NULL);
	Lazors.Add(_source, lazor);

	// Create the target
	AActor* target = GetWorld()->SpawnActor(ALazorTarget::StaticClass());

	// Search for the target's position
	FHitResult hit;
	FVector dir = _inDir;
	FVector end = _inPos + dir * 100000; // Relative is used because the LazorManager is centered (pos={0,0,0})
	bool hasHit = GetWorld()->LineTraceSingleByChannel(hit, _inPos, end, ECC_PhysicsBody);
	if (hasHit)
		target->SetActorLocation(hit.ImpactPoint);
	else
		target->SetActorLocation(end);

	ParticleSystemTargets.Add(particleSystem, target); // TO REMOVE IN FAVOR OF LAZOR IMPLEMENTATION

	lazor->SetTarget(target);
	particleSystem->SetActorParameter("Target", target);

	// If the _source is movable we register to it
	{
		UMovable* movable = NULL;

		TArray<UActorComponent*> components = _source->GetComponents();
		for (UActorComponent* comp : components)
		{
			if (comp->GetClass()->IsChildOf<UMovable>())
			{
				// _source is a movable object
				movable = CastChecked<UMirror>(comp);
				break;
			}
		}

		if (movable != NULL)
		{
			movable->AddEventHandler(this);
		}
	}

	// Now we need to determine if we launch a new beam
	if (hasHit && hit.Actor.IsValid())
	{
		AActor* actor = hit.GetActor();
		UMirror* mirror = NULL;

		TArray<UActorComponent*> components = actor->GetComponents();
		for (UActorComponent* comp : components)
		{
			if (comp->GetClass()->IsChildOf<UMirror>())
			{
				// found a mirror !
				mirror = CastChecked<UMirror>(comp);
				break;
			}
		}

		if (mirror != NULL)
		{
			// We need to bounce the lazor
			Lazor* l = CreateLazor(hit.ImpactPoint, dir.MirrorByVector(mirror->GetNormal()), actor);
			lazor->SetChild(l);
			l->SetParent(lazor);
		}
	}

	return lazor;
}

void ULazorManager::UpdateLazor(Lazor* _lazor)
{

}

UParticleSystemComponent* ULazorManager::InstantiateParticleSystem(const FVector& _location, const FRotator& _direction)
{
	UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAttached(ParticleTemplate, this, NAME_None, _location, _direction);
	FParticleSysParam param;
	param.Name = "Target";
	param.ParamType = EParticleSysParamType::PSPT_Actor;
	particleSystem->InstanceParameters.Add(param);
	particleSystem->UpdateInstances(true);
	particleSystem->ActivateSystem();

	return particleSystem;
}

void ULazorManager::OnMove(const UMovable* _sender)
{

}