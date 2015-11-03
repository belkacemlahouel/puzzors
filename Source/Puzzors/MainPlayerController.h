// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PUZZORS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	bool bShowCursor;
	FIntPoint MousePosSave;

	UPROPERTY(EditAnywhere)
		float Range;


protected:
	void SetupInputComponent() override;

private:
	void ToggleCursor();
	void Interact();
	
	
};
