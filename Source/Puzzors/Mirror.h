// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Movable.h"
#include "Mirror.generated.h"

/**
 * 
 */
UCLASS( Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class PUZZORS_API UMirror : public UMovable
{
	GENERATED_BODY()

	UMirror();

public:
	FVector GetNormal() const;

protected:
	virtual void Rotate_Implementation(const FRotator& _Deltas) override;
	virtual void Translate_Implementation(const FVector& _Deltas) override;

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
