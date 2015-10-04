// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "RotableActor.h"
#include "PlayerPawn.generated.h"

UCLASS()
class PUZZORS_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveUp(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	void ToggleCursor();
	void Interact();

protected:
	bool bShowCursor;
	FIntPoint MousePosSave;

	UCameraComponent* Camera;
	FVector MovementInput;
	FVector2D CameraInput;

	UPROPERTY(EditAnywhere)
		float MaxSpeed;
	UPROPERTY(EditAnywhere)
		float RotSpeed;

	UPROPERTY(EditAnywhere)
		float Range;
};
