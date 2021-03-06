// Fill out your copyright notice in the Description page of Project Settings.

#include "Puzzors.h"
#include "PlayerPawn.h"

#include "Movable.h"

// Sets default values
APlayerPawn::APlayerPawn() : bShowCursor(false), mController(NULL), mTarget(NULL)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	Camera->AttachTo(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//xoxo
	previousLocation = FVector(0);
	previousRotation = FQuat();
	isOnTop = false;
	PrecisionMode = false;
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
		if (!isOnTop)
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
	}
	else
	{
		if (mMousePressed && mTarget != NULL)
		{
			FVector pos, dir;
			if(mController->DeprojectMousePositionToWorld(pos, dir))
			{
				if (InteractionType == EInteractionType::IT_Rotation)
				{
					FVector forward = FVector(dir.X, dir.Y, 0);
					forward.Normalize();
					FVector right = FVector::CrossProduct(FVector::UpVector, forward);
					right.Normalize();
					right = mTarget->GetOwner()->GetActorRotation().UnrotateVector(right);

					float tweak = PrecisionMode ? RotationTweaker / 10.0f : RotationTweaker;

					FQuat quat(right, FMath::DegreesToRadians(CameraInput.Y*tweak));
					
					FRotator rot(quat.Rotator().Pitch, -CameraInput.X*tweak, quat.Rotator().Roll);
					
					if (!rot.IsNearlyZero())
						mTarget->Move(rot, FVector::ZeroVector);
				}
				else if (InteractionType == EInteractionType::IT_Translation)
				{
					FVector forward = FVector(dir.X, dir.Y, 0);
					forward.Normalize();
					FVector right = FVector::CrossProduct(FVector::UpVector, forward);
					right.Normalize();

					float tweak = PrecisionMode ? TranslationTweaker / 10.0f : TranslationTweaker;
					FVector move = forward * CameraInput.Y * tweak + right * CameraInput.X * tweak;
					mTarget->Move(FRotator::ZeroRotator, move);
				}
			}
		}
	}

	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
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
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerPawn::InteractStart);
	InputComponent->BindAction("Interact", IE_Released, this, &APlayerPawn::InteractEnd);
	InputComponent->BindAction("TogglePrecisionMode", IE_Pressed, this, &APlayerPawn::TogglePrecisionMode);
	InputComponent->BindAction("TogglePrecisionMode", IE_Released, this, &APlayerPawn::TogglePrecisionMode);

	//xoxo
	InputComponent->BindAction("SwitchCamera", IE_Released, this, &APlayerPawn::SwitchCamera);

	InputComponent->BindAction("ChangeInteractionType", IE_Released, this, &APlayerPawn::ChangeInteractionType);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &APlayerPawn::MoveUp);
	InputComponent->BindAxis("CameraPitch", this, &APlayerPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APlayerPawn::YawCamera);
}

void APlayerPawn::ChangeInteractionType()
{
	if (InteractionType == EInteractionType::IT_Rotation)
		InteractionType = EInteractionType::IT_Translation;
	else
		InteractionType = EInteractionType::IT_Rotation;
}

void APlayerPawn::MoveForward(float AxisValue)
{
	//xoxo
	if (isOnTop)
		return;

	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::MoveRight(float AxisValue)
{
	//xoxo
	if (isOnTop)
		return;

	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APlayerPawn::MoveUp(float AxisValue)
{
	//xoxo
	if (isOnTop)
		return;

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
			mController = controller;
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

void APlayerPawn::InteractStart()
{
	if (!bShowCursor) return;

	if (this->GetController()->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact Start"));
		mMousePressed = true;

		APlayerController* controller = mController;
		FVector pos, dir;
		if (controller->DeprojectMousePositionToWorld(pos, dir))
		{
			FHitResult hit;
			if (GetWorld()->LineTraceSingleByChannel(hit, pos, pos + dir * Range, ECC_PhysicsBody))
			{
				FString name = hit.GetActor()->GetName();
				UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Ray Hit - %s"), *name);
				TArray<UMovable*> comp;
				hit.GetActor()->GetComponents<UMovable>(comp);
				if (comp.Num() > 0)
				{
					mTarget = comp[0];
				}
				else
				{
					mTarget = NULL;
				}
			}
			else
			{
				mTarget = NULL;
			}
		}
	}
}

void APlayerPawn::InteractEnd()
{
	mTarget = NULL;
	mMousePressed = false;
	UE_LOG(LogTemp, Warning, TEXT("<APlayerPawn>: Interact End"));
}

//xoxo
void APlayerPawn::SwitchCamera()
{
	if (isOnTop)
	{
		Camera->SetWorldRotation(previousRotation);
		SetActorLocation(previousLocation);
		previousRotation = FQuat();
		previousLocation = FVector(0);
		isOnTop = false;
	}
	else
	{
		previousLocation = GetActorLocation();
		previousRotation = Camera->GetComponentRotation().Quaternion();
		FVector myLoc = FVector(0, 0, 5000);
		SetActorLocation(myLoc); //on-top location
		FRotator myRot(-90, 0, 0);
		Camera->SetWorldRotation(myRot.Quaternion()); //looking bot
		isOnTop = true;
	}
}

void APlayerPawn::TogglePrecisionMode()
{
	PrecisionMode = !PrecisionMode;
}