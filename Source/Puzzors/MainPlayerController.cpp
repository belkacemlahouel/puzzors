// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "MainPlayerController.h"

void AMainPlayerController::SetupInputComponent()
{
	
}

void AMainPlayerController::ToggleCursor()
{
	if (IsLocalPlayerController())
	{
		if (!bShowCursor)
		{
			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			FViewport* v = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
			v->SetMouse(MousePosSave.X, MousePosSave.Y);

			bShowCursor = true;
		}
		else
		{
			bShowMouseCursor = false;
			bEnableClickEvents = false;
			bEnableMouseOverEvents = false;

			FViewport* v = CastChecked<ULocalPlayer>(Player)->ViewportClient->Viewport;
			v->GetMousePos(MousePosSave);

			bShowCursor = false;
		}
	}
}

void AMainPlayerController::Interact()
{
	if (!bShowCursor) return;

	if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact Start"));
		FVector pos, dir;
		if (DeprojectMousePositionToWorld(pos, dir))
		{
			FHitResult hit;
			if (GetWorld()->LineTraceSingleByChannel(hit, pos, pos + dir * Range, ECC_PhysicsBody))
			{
				FString name = hit.GetActor()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Ray Hit - %s"), *name);
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact End"));
	}
}


