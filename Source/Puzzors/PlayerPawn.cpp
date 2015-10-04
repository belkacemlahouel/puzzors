// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "PlayerPawn.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->AttachTo(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bShowCursor = false;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (this->GetController() && this->GetController()->IsLocalPlayerController())
	{
		APlayerController* controller = GetController()->CastToPlayerController();
		FViewport* v = CastChecked<ULocalPlayer>(controller->Player)->ViewportClient->Viewport;
		MousePosSave = v->GetSizeXY();
		MousePosSave.X /= 2.0f;
		MousePosSave.Y /= 2.0f;
	}
}

// Called every frame
void APlayerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!bShowCursor)
	{
		{
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw += CameraInput.X * RotSpeed;
			SetActorRotation(NewRotation);
		}

		{
			FRotator NewRotation = Camera->GetComponentRotation();
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y * RotSpeed, -80.0f, 80.0f);
			Camera->SetWorldRotation(NewRotation);
		}
	}

	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.SafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime * MaxSpeed;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime * MaxSpeed;
			NewLocation += GetActorUpVector() * MovementInput.Z * DeltaTime * MaxSpeed;
			SetActorLocation(NewLocation);
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//Hook show cursor handle
	InputComponent->BindAction("ToggleCursor", IE_Released, this, &APlayerPawn::ToggleCursor);
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerPawn::Interact);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &APlayerPawn::MoveUp);
	InputComponent->BindAxis("CameraPitch", this, &APlayerPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APlayerPawn::YawCamera);
}

void APlayerPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::MoveUp(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APlayerPawn::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APlayerPawn::ToggleCursor()
{
	if (this->GetController()->IsLocalPlayerController())
	{
		if (!bShowCursor)
		{
			APlayerController* controller = GetController()->CastToPlayerController();
			controller->bShowMouseCursor = true;
			controller->bEnableClickEvents = true;
			controller->bEnableMouseOverEvents = true;

			FViewport* v = CastChecked<ULocalPlayer>(controller->Player)->ViewportClient->Viewport;
			v->SetMouse(MousePosSave.X, MousePosSave.Y);

			bShowCursor = true;
		}
		else
		{
			APlayerController* controller = GetController()->CastToPlayerController();
			controller->bShowMouseCursor = false;
			controller->bEnableClickEvents = false;
			controller->bEnableMouseOverEvents = false;

			FViewport* v = CastChecked<ULocalPlayer>(controller->Player)->ViewportClient->Viewport;
			v->GetMousePos(MousePosSave);

			bShowCursor = false;
		}
	}
}

void APlayerPawn::Interact()
{
	if (!bShowCursor) return;

	if (this->GetController()->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact Start"));
		APlayerController* controller = GetController()->CastToPlayerController();
		FVector pos, dir;
		if (controller->DeprojectMousePositionToWorld(pos, dir))
		{
			FHitResult hit;
			if (GetWorld()->LineTraceSingleByChannel(hit, pos, pos + dir * Range, ECC_PhysicsBody))
			{
				FString name = hit.GetActor()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Ray Hit - %s"), *name);
				TArray<URotableActor*> comp;
				hit.GetActor()->GetComponents<URotableActor>(comp);
				if (comp.Num() > 0)
				{
					comp[0]->Rotate();
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact End"));
	}
}