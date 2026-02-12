// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "Character/HexploreCharacter.h"
#include "Character/HexploreCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	InputModeData.SetHideCursorDuringCapture(true);
	SetInputMode(InputModeData);
}

void AHexplorePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UHexploreInputComponent* HexploreInputComponent = CastChecked<UHexploreInputComponent>(InputComponent);

	HexploreInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::Move);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHexplorePlayerController::SprintStart);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::SprintEnd);

	// Mouse Input
	HexploreInputComponent->BindAction(RMBAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBStarted);
	HexploreInputComponent->BindAction(RMBAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBCompleted);
	HexploreInputComponent->BindAction(LMBAction, ETriggerEvent::Started, this, &AHexplorePlayerController::LMBStarted);
	HexploreInputComponent->BindAction(LMBAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::LMBCompleted);
	HexploreInputComponent->BindAction(MouseDeltaAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::MouseDelta);
	HexploreInputComponent->BindAction(ScrollWheelAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::ScrollWheel);
	
	HexploreInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);

	// RMB hold (Camera), leaving that on the side for now
	// HexploreInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBPressed);
	// HexploreInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBReleased);
}

USpringArmComponent* AHexplorePlayerController::GetSpringArm()
{
	if (CachedSpringArm == nullptr)
	{
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			AHexploreCharacter* HexploreCharacter = Cast<AHexploreCharacter>(ControlledPawn);
			USpringArmComponent* SpringArm = HexploreCharacter->FindComponentByClass<USpringArmComponent>();
			CachedSpringArm = SpringArm;
		}
	}
	return CachedSpringArm;
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
		Movement->bOrientRotationToMovement = true;
		Movement->bUseControllerDesiredRotation = false;
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

void AHexplorePlayerController::LMBStarted(const FInputActionValue& InputActionValue)
{
	bIsLMBDown = true;
}

void AHexplorePlayerController::LMBCompleted(const FInputActionValue& InputActionValue)
{
	bIsLMBDown = false;
	if (bIsLMBDragging) bIsLMBDragging = false;
}

void AHexplorePlayerController::RMBStarted(const FInputActionValue& InputActionValue)
{
	bIsRMBDown = true;
}

void AHexplorePlayerController::RMBCompleted(const FInputActionValue& InputActionValue)
{
	bIsRMBDown = false;
	if (bIsRMBDragging) bIsRMBDragging = false;

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		AHexploreCharacter* HexploreCharacter = Cast<AHexploreCharacter>(ControlledPawn);
		HexploreCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		HexploreCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	}
	
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	AHexploreCharacter* HexploreCharacter = GetPawn<AHexploreCharacter>();
	if (!HexploreCharacter) return;

	AActor* HitActor = CursorHit.GetActor();

	// Only proceed if it's a valid target and different from current
	if (Cast<ITargetInterface>(HitActor) && HitActor != HexploreCharacter->GetCombatTarget())
	{
		HexploreCharacter->SetCombatTarget(HitActor);
	}
}

void AHexplorePlayerController::MouseDelta(const FInputActionValue& InputActionValue)
{
	AHexploreCharacter* HexploreCharacter = GetPawn<AHexploreCharacter>();

	if (bIsLMBDown)
	{
		bIsLMBDragging = true;
		FVector2D MouseAxis = InputActionValue.Get<FVector2D>();
		
		AddYawInput(MouseAxis.X * SensitivityMultiplier);
		AddPitchInput(MouseAxis.Y * SensitivityMultiplier);
	}

	if (bIsRMBDown)
	{
		bIsRMBDragging = true;
		HexploreCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		HexploreCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		FVector2D MouseAxis = InputActionValue.Get<FVector2D>();
		
		AddYawInput(MouseAxis.X * SensitivityMultiplier);
		AddPitchInput(MouseAxis.Y * SensitivityMultiplier);
	}
}

void AHexplorePlayerController::ScrollWheel(const FInputActionValue& InputActionValue)
{
	float ScrollValue = InputActionValue.Get<float>();
	float ZoomValue = GetSpringArm()->TargetArmLength;
	if (ScrollValue > 0) ZoomValue += -100.f;
	else ZoomValue += 100.f;
	GetSpringArm()->TargetArmLength = FMath::Clamp(ZoomValue, 200.f, 800.f);
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
