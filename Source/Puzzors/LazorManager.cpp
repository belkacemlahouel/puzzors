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
		DeleteObject(var);
	}
	Lazors.Empty();
}


// Called every frame
void ULazorManager::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

ULazor* ULazorManager::CreateLazor(const FVector& _inPos, const FVector& _inDir, const AActor* _source)
{
	return CreateLazorInternal(_inPos, _inDir, _source, NULL);
}

ULazor* ULazorManager::CreateLazorInternal(const FVector& _inPos, const FVector& _inDir, const AActor* _source, ULazor* _parent)
{
	// Create Particle system
	UParticleSystemComponent* particleSystem = InstantiateParticleSystem(_inPos, FRotator::ZeroRotator);

	// Create the lazor
	ULazor* lazor = InstanciateLazor(particleSystem, _source, _parent);
	lazor->SetPosition(_inPos);
	lazor->SetDirection(_inDir);
	Lazors.Add(lazor);

	FHitResult hit;
	AActor* target = ComputeLazorTarget(_inPos, _inDir, hit);
	bool hasHit = hit.IsValidBlockingHit();
	lazor->SetTarget(target);
	particleSystem->SetActorParameter("Target", target);

	// If the _source is movable we register to it
	//{
	//	UMovable* movable = NULL;

	//	TArray<UActorComponent*> components = _source->GetComponents();
	//	for (UActorComponent* comp : components)
	//	{
	//		if (comp->GetClass()->IsChildOf<UMovable>())
	//		{
	//			// _source is a movable object
	//			movable = CastChecked<UMirror>(comp);
	//			break;
	//		}
	//	}

	//	if (movable != NULL)
	//	{
	//		movable->AddEventHandler(this);
	//		if (!LazorTracking.Contains(movable))
	//			LazorTracking.Add(movable);

	//		if (!LazorTracking[movable].Contains(lazor))
	//			LazorTracking[movable].Add(lazor);
	//	}
	//}

	// Now we need to determine if we launch a new beam
	if (hasHit && hit.Actor.IsValid() && lazor->Index() < 10)
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
			// first we register to the mirror's movements
			mirror->AddEventHandler(this);
			if (!LazorTracking.Contains(mirror))
				LazorTracking.Add(mirror);

			if (!LazorTracking[mirror].Contains(lazor))
				LazorTracking[mirror].Add(lazor);

			// We need to bounce the lazor
			CreateLazorInternal(hit.ImpactPoint, _inDir.MirrorByVector(mirror->GetNormal()), actor, lazor);
		}
	}

	return lazor;
}

void ULazorManager::UpdateLazor(ULazor* _lazor)
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

ULazor* ULazorManager::InstanciateLazor(UParticleSystemComponent* _particle, const AActor* _source, ULazor* _parent)
{
	ULazor* lazor = NewObject<ULazor>();
	lazor->SetParticleSystem(_particle);
	lazor->SetSource(_source);
	lazor->SetParent(_parent);
	if(_parent != NULL) _parent->SetChild(lazor);
	return lazor;
}

AActor* ULazorManager::ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult)
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

void ULazorManager::OnMove(const UMovable* _sender)
{
	if (LazorTracking.Contains(_sender))
	{
		TArray<ULazor*>& lazors = LazorTracking[_sender];
		TArray<ULazor*> MarkedForDeletion;
		TArray<ULazor*> MarkedForRecreation;
		for (int i = 0; i < lazors.Num(); ++i)
		{
			// Two case, lazors[i] can be a child of an other lazor
			//			 or lazors[i] can be a parent of an other lazor
			
			if (MarkedForDeletion.Contains(lazors[i])) // lazors[i] is the child of a previously processed lazor
				continue;

			MarkedForRecreation.Add(lazors[i]);
			ULazor* l = lazors[i]->Child();
			while (l != NULL)
			{
				if (!MarkedForDeletion.Contains(l))
					MarkedForDeletion.Add(l);
				
				if (MarkedForRecreation.Contains(l)) // l is on the MovableObject with its parent
					MarkedForRecreation.Remove(l);

				l = l->Child();
			}
		}

		lazors.Empty();

		for (int i = 0; i < MarkedForDeletion.Num(); ++i)
		{
			DestroyLazor(MarkedForDeletion[i]);
		}
		MarkedForDeletion.Empty();

		for (int i = 0; i < MarkedForRecreation.Num(); ++i)
		{
			ULazor* lazor = MarkedForRecreation[i];
			// either the source or the target has moved
			if (_sender->IsA<UMirror>())
			{
				CreateLazorInternal(lazor->Position(), lazor->Direction(), lazor->Source(), lazor->Parent());
			}

			DestroyLazor(MarkedForRecreation[i]);
		}
		MarkedForRecreation.Empty();
	}
}

void ULazorManager::DestroyLazor(ULazor* _lazor)
{
	_lazor->Target()->Destroy();
	_lazor->ParticleSystem()->DestroyComponent();
	Lazors.Remove(_lazor);
	DeleteObject(_lazor);
}