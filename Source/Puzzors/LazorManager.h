// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MoveEventHandler.h"
#include "Components/SceneComponent.h"
#include "LazorManager.generated.h"

class ULazor;
class ABeam;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZORS_API ULazorManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULazorManager();

	virtual void BeginDestroy() override;
	
	// Create a Lazor emitted by _source actor, at _inPos position and going to _inDir direction.
	UFUNCTION(BlueprintCallable, Category = "Lazor Creation")
		ABeam* CreateBeam(const FVector& _inPos, const FVector& _inDir);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
		UParticleSystem* ParticleTemplate;

	TArray<ABeam*> Beams;

};
