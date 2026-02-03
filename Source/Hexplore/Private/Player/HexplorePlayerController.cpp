// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/HexploreCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/TargetInterface.h"

AHexplorePlayerController::AHexplorePlayerController()
{
	bReplicates = true;
}

void AHexplorePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::Sprint);

	// RMB hold (Camera), leaving that on the side for now
	// EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBPressed);
	// EnhancedInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBReleased);
}

void AHexplorePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AHexplorePlayerController::Sprint(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		AHexploreCharacter* HexploreCharacter = Cast<AHexploreCharacter>(ControlledPawn);
		UCharacterMovementComponent* Movement = HexploreCharacter->GetCharacterMovement();
		Movement->MaxWalkSpeed = 600.f;
	}
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
