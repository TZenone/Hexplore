// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/HexploreCharacter.h"
#include "Interaction/TargetInterface.h"

AHexplorePlayerController::AHexplorePlayerController()
{
	bReplicates = true;
}

void AHexplorePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if (bRotateCameraHeld)
	{
		UpdateControlRotationToCursor(DeltaTime);
	}
}

void AHexplorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(HexploreContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(HexploreContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AHexplorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::Move);

	// RMB hold (Camera)
	EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBPressed);
	EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBReleased);
}

void AHexplorePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AHexplorePlayerController::RMBPressed(const FInputActionValue& Value)
{
	bRotateCameraHeld = true;

	if (AHexploreCharacter* Char = Cast<AHexploreCharacter>(GetPawn()))
	{
		Char->SetCameraMode(true);
	}
}

void AHexplorePlayerController::RMBReleased(const FInputActionValue& Value)
{
	bRotateCameraHeld = false;

	if (AHexploreCharacter* Char = Cast<AHexploreCharacter>(GetPawn()))
	{
		Char->SetCameraMode(false);
	}
}

void AHexplorePlayerController::UpdateControlRotationToCursor(float DeltaTime)
{
	APawn* P = GetPawn();
	if (!P) return;
	
	int32 SizeX = 0, SizeY = 0;
	GetViewportSize(SizeX, SizeY);
	const FVector2D ViewCenter(SizeX * 0.5f, SizeY * 0.5f);

	float MouseX = 0.f, MouseY = 0.f;
	if (!GetMousePosition(MouseX, MouseY)) return;

	const FVector2D MousePos(MouseX, MouseY);
	if (FVector2D::Distance(MousePos, ViewCenter) <= CenterDeadZoneRadiusPx)
	{
		// Inside dead zone: keep current yaw (no micro-jitter)
		return;
	}

	// --- CURSOR HIT ---
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	const FVector PawnLocation = P->GetActorLocation();
	const FVector Target = CursorHit.ImpactPoint;

	const float DesiredYaw = (Target - PawnLocation).Rotation().Yaw;
	const float CurrentYaw = GetControlRotation().Yaw;

	const float Dist = FVector2D::Distance(MousePos, ViewCenter);
	const float EffectiveDist = FMath::Max(0.f, Dist - CenterDeadZoneRadiusPx);
	
	const float MaxDist = 0.5f * FMath::Min(SizeX, SizeY);
	const float Alpha = FMath::Clamp(EffectiveDist / MaxDist, 0.f, 1.f);
	
	const float TurnSpeed = FMath::Lerp(0.f, AimYawDegreesPerSecond, Alpha);
	const float NewYaw = FMath::FixedTurn(CurrentYaw, DesiredYaw, TurnSpeed * DeltaTime);

	FRotator NewRot = GetControlRotation();
	NewRot.Yaw = NewYaw;
	SetControlRotation(NewRot);
}

void AHexplorePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	// Line trace from cursor, there are several scenarios:
	// A. LastActor is null and ThisActor is null -> do nothing
	// B. LastActor is null and ThisActor is valid -> highlight ThisActor
	// C. LastActor is valid and ThisActor is null -> unhighlight LastActor
	// D. Both actors are valid but LastActor is different from ThisActor -> Unhighlight LastActor and Highlight ThisActor
	// E. Both actors are valid and are the same actor -> do nothing

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}
