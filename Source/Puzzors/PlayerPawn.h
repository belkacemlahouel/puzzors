// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UMovable;

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	IT_Rotation UMETA(DisplayName = "Rotation"),
	IT_Translation UMETA(DisplayName = "Translation"),
};

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
	void InteractStart();
	void InteractEnd();

	//xoxo
	void SwitchCamera();

protected:
	bool bShowCursor;
	FIntPoint MousePosSave;

	UCameraComponent* Camera;
	FVector MovementInput;
	FVector2D CameraInput;

	//xoxo
	FVector previousLocation;
	FQuat previousRotation;
	bool isOnTop;

	UPROPERTY(EditAnywhere)
		float MaxSpeed;
	UPROPERTY(EditAnywhere)
		float RotSpeed;

	UPROPERTY(EditAnywhere)
		float RotationTweaker;

	UPROPERTY(EditAnywhere)
		float TranslationTweaker;

	UPROPERTY(EditAnywhere)
		float Range;

private:
	APlayerController* mController;
	bool mMousePressed;
	UMovable* mTarget;

	UPROPERTY(EditAnywhere)
		EInteractionType InteractionType;
};
