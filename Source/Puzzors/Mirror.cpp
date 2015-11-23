// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "Mirror.h"

UMirror::UMirror() : UMovable(), m_TranslationAccumulator(0, 0, 0) {}

FVector UMirror::GetNormal() const
{
	FRotator rot = GetOwner()->GetActorRotation();
	return rot.RotateVector(Normal);
}

void UMirror::Rotate_Implementation(const FRotator& _Deltas)
{
	FRotator rot = GetOwner()->GetActorRotation();
	rot.Add(_Deltas.Pitch, _Deltas.Yaw, _Deltas.Roll);
	ClampRotation(rot);
	GetOwner()->SetActorRotation(rot);
}

void UMirror::Translate_Implementation(const FVector& _Deltas)
{
	FVector delta = _Deltas;
	ClampTranslation(delta);
	delta += GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(delta);
}

void UMirror::ClampRotation(FRotator& _Rotation)
{
	if (_Rotation.Pitch > RotationLibertyMax.Pitch)
	{
		_Rotation.Pitch = RotationLibertyMax.Pitch;
	}
	else if (_Rotation.Pitch < RotationLibertyMin.Pitch)
	{
		_Rotation.Pitch = RotationLibertyMin.Pitch;
	}

	if (_Rotation.Yaw > RotationLibertyMax.Yaw)
	{
		_Rotation.Yaw = RotationLibertyMax.Yaw;
	}
	else if (_Rotation.Yaw < RotationLibertyMin.Yaw)
	{
		_Rotation.Yaw = RotationLibertyMin.Yaw;
	}

	if (_Rotation.Roll > RotationLibertyMax.Roll)
	{
		_Rotation.Roll = RotationLibertyMax.Roll;
	}
	else if (_Rotation.Roll < RotationLibertyMin.Roll)
	{
		_Rotation.Roll = RotationLibertyMin.Roll;
	}
}

void UMirror::ClampTranslation(FVector& _Translation)
{
	FVector newAcc = m_TranslationAccumulator + _Translation;

	if (newAcc.X > TranslationLibertyMax.X)
	{
		newAcc.X = TranslationLibertyMax.X;
	}
	else if (newAcc.X < TranslationLibertyMin.X)
	{
		newAcc.X = TranslationLibertyMin.X;
	}

	if (newAcc.Y > TranslationLibertyMax.Y)
	{
		newAcc.Y = TranslationLibertyMax.Y;
	}
	else if (newAcc.Y < TranslationLibertyMin.Y)
	{
		newAcc.Y = TranslationLibertyMin.Y;
	}

	if (newAcc.Z > TranslationLibertyMax.Z)
	{
		newAcc.Z = TranslationLibertyMax.Z;
	}
	else if (newAcc.Z < TranslationLibertyMin.Z)
	{
		newAcc.Z = TranslationLibertyMin.Z;
	}

	_Translation.Set(newAcc.X - m_TranslationAccumulator.X,
						newAcc.Y - m_TranslationAccumulator.Y,
						newAcc.Z - m_TranslationAccumulator.Z);

	m_TranslationAccumulator += _Translation;
}