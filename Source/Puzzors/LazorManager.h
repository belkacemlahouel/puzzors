// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MoveEventHandler.h"
#include "Components/SceneComponent.h"
#include "LazorManager.generated.h"

class ULazor;
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
	UFUNCTION(BlueprintCallable, Category = "Lazor Creation")
		ULazor* CreateLazor(const FVector& _inPos, const FVector& _inDir, const AActor* _source);

	void UpdateLazor(ULazor* _lazor);

	void OnMove(const UMovable* _sender);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		UParticleSystem* ParticleTemplate;

	TArray<ULazor*> Lazors;

	TMap<const UMovable*, TArray<ULazor*>> LazorTracking;
	
private:
	UParticleSystemComponent* InstantiateParticleSystem(const FVector& _location, const FRotator& _direction);
	ULazor* InstanciateLazor(UParticleSystemComponent* _particle, const AActor* _source, ULazor* _parent);

	ULazor* CreateLazorInternal(const FVector& _inPos, const FVector& _inDir, const AActor* _source, ULazor* _parent);
	AActor* ComputeLazorTarget(const FVector& _inPos, const FVector& _inDir, FHitResult& _outHitResult);

	void DestroyLazor(ULazor* _lazor);

};
