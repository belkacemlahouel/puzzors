// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Movable.h"
#include "Mirror.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PUZZORS_API UMirror : public UMovable
{
	GENERATED_BODY()

	UMirror();

public:
	FVector GetNormal() const;

protected:
	void Rotate(const FRotator& _Deltas);
	void Translate(const FVector& _Deltas);

private:
	void ClampRotation(FRotator& _Rotation);
	void ClampTranslation(FVector& _Translation);

private:
	UPROPERTY(EditAnywhere)
		FVector Normal;

	UPROPERTY(EditAnywhere)
		FRotator RotationLibertyMin;

	UPROPERTY(EditAnywhere)
		FRotator RotationLibertyMax;

	UPROPERTY(EditAnywhere)
		FVector TranslationLibertyMin;

	UPROPERTY(EditAnywhere)
		FVector TranslationLibertyMax;

	FVector m_InitialLocation;
	FVector m_TranslationAccumulator;
};
