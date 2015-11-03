// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MoveEventHandler.h"
#include "Components/SceneComponent.h"
#include "LazorManager.generated.h"

class Lazor;
class UMirror;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZORS_API ULazorManager : public USceneComponent, public IMoveEventHandler
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULazorManager();

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Create a Lazor emitted by _source actor, at _inPos position and going to _inDir direction.
	Lazor* CreateLazor(const FVector& _inPos, const FVector& _inDir, const AActor* _source);

	void UpdateLazor(Lazor* _lazor);

	void OnMove(const UMovable* _sender);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		UParticleSystem* ParticleTemplate;

	TMap<const AActor*, UParticleSystemComponent*> ParticleSystemComponents;
	TMap<UParticleSystemComponent*, AActor*> ParticleSystemTargets;

	TMap<const AActor*, Lazor*> Lazors;
	
private:
	UParticleSystemComponent* InstantiateParticleSystem(const FVector& _location, const FRotator& _direction);

};
