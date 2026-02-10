// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "Character/HexploreCharacter.h"
#include "Character/HexploreCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Input/HexploreInputComponent.h"
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
	if (Subsystem)
	{
		Subsystem->AddMappingContext(HexploreContext, 0);
	}

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

	UHexploreInputComponent* HexploreInputComponent = CastChecked<UHexploreInputComponent>(InputComponent);

	HexploreInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::Move);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHexplorePlayerController::SprintStart);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::SprintEnd);
	HexploreInputComponent->BindAction(RMBAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMB);

	HexploreInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

	// RMB hold (Camera), leaving that on the side for now
	// HexploreInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBPressed);
	// HexploreInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBReleased);
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

void AHexplorePlayerController::SprintStart(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		AHexploreCharacter* HexploreCharacter = Cast<AHexploreCharacter>(ControlledPawn);
		UCharacterMovementComponent* Movement = HexploreCharacter->GetCharacterMovement();
		Movement->MaxWalkSpeed = 600.f;
	}
}

void AHexplorePlayerController::SprintEnd(const FInputActionValue& InputActionValue)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		AHexploreCharacter* HexploreCharacter = Cast<AHexploreCharacter>(ControlledPawn);
		UCharacterMovementComponent* Movement = HexploreCharacter->GetCharacterMovement();
		Movement->MaxWalkSpeed = 300.f;
	}
}

void AHexplorePlayerController::RMB(const FInputActionValue& InputActionValue)
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	AHexploreCharacter* HexploreCharacter = GetPawn<AHexploreCharacter>();
	if (!HexploreCharacter) return;

	AActor* HitActor = CursorHit.GetActor();

	// Only proceed if it's a valid target and different from current
	if (Cast<ITargetInterface>(HitActor) && HitActor != HexploreCharacter->GetCurrentTarget())
	{
		HexploreCharacter->SetCurrentTarget(HitActor);
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

void AHexplorePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
}

void AHexplorePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AHexplorePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UHexploreAbilitySystemComponent* AHexplorePlayerController::GetASC()
{
	if (HexploreAbilitySystemComponent == nullptr)
	{
		HexploreAbilitySystemComponent = Cast<UHexploreAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HexploreAbilitySystemComponent;
}
