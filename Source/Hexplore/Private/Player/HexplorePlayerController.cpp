// Copyright Tristan ZENONE.

#include "Player/HexplorePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "Character/HexploreCharacter.h"
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

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
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

	// Movement
	HexploreInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::Move);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AHexplorePlayerController::SprintStart);
	HexploreInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::SprintEnd);

	// Mouse
	HexploreInputComponent->BindAction(LMBAction, ETriggerEvent::Started, this, &AHexplorePlayerController::LMBStarted);
	HexploreInputComponent->BindAction(LMBAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::LMBCompleted);
	HexploreInputComponent->BindAction(RMBAction, ETriggerEvent::Started, this, &AHexplorePlayerController::RMBStarted);
	HexploreInputComponent->BindAction(RMBAction, ETriggerEvent::Completed, this, &AHexplorePlayerController::RMBCompleted);
	HexploreInputComponent->BindAction(MouseDeltaAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::MouseDelta);
	HexploreInputComponent->BindAction(ScrollWheelAction, ETriggerEvent::Triggered, this, &AHexplorePlayerController::ScrollWheel);

	// Abilities
	HexploreInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

/*
 * Cached Getters
 */

USpringArmComponent* AHexplorePlayerController::GetSpringArm()
{
	if (CachedSpringArm == nullptr)
	{
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			CachedSpringArm = ControlledPawn->FindComponentByClass<USpringArmComponent>();
		}
	}
	return CachedSpringArm;
}

UHexploreAbilitySystemComponent* AHexplorePlayerController::GetASC()
{
	if (HexploreAbilitySystemComponent == nullptr)
	{
		HexploreAbilitySystemComponent = Cast<UHexploreAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HexploreAbilitySystemComponent;
}

/*
 * Character Movement State
 */

void AHexplorePlayerController::UpdateRotationMode()
{
	AHexploreCharacter* PlayerCharacter = GetPawn<AHexploreCharacter>();
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();

	const bool bShouldLockToController = !bIsSprinting && (PlayerCharacter->GetCombatTarget() != nullptr || bIsRMBDragging);

	Movement->bOrientRotationToMovement = !bShouldLockToController;
	Movement->bUseControllerDesiredRotation = bShouldLockToController;
}

/*
 * Movement Input
 */

void AHexplorePlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

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
	AHexploreCharacter* PlayerCharacter = GetPawn<AHexploreCharacter>();
	if (!PlayerCharacter) return;

	bIsSprinting = true;
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	
	const FGameplayTag FleeingTag = FGameplayTag::RequestGameplayTag(FName("Status.General.Fleeing"));
	UE_LOG(LogTemp, Warning, TEXT("[%s] Tag Added to [%s]"), *FleeingTag.GetTagName().ToString() , *PlayerCharacter->GetName());
	PlayerCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FleeingTag);
	const FGameplayTag ImmuneTag = FGameplayTag::RequestGameplayTag(FName("Status.General.ImmuneToOpportunityAction"));
	PlayerCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(ImmuneTag);
	UpdateRotationMode();
}

void AHexplorePlayerController::SprintEnd(const FInputActionValue& InputActionValue)
{
	AHexploreCharacter* PlayerCharacter = GetPawn<AHexploreCharacter>();
	if (!PlayerCharacter) return;

	bIsSprinting = false;
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
	const FGameplayTag FleeingTag = FGameplayTag::RequestGameplayTag(FName("Status.General.Fleeing"));
	PlayerCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FleeingTag);
	UpdateRotationMode();
}

/*
 * Mouse Input
 */

void AHexplorePlayerController::LMBStarted(const FInputActionValue& InputActionValue)
{
	bIsLMBDown = true;
}

void AHexplorePlayerController::LMBCompleted(const FInputActionValue& InputActionValue)
{
	if (!bIsLMBDragging)
	{
		// Tap: clear combat target
		AHexploreCharacter* PlayerCharacter = GetPawn<AHexploreCharacter>();
		if (PlayerCharacter && PlayerCharacter->GetCombatTarget() != nullptr)
		{
			PlayerCharacter->ClearCombatTarget();
			UpdateRotationMode();
		}
	}

	bIsLMBDown = false;
	bIsLMBDragging = false;
}

void AHexplorePlayerController::RMBStarted(const FInputActionValue& InputActionValue)
{
	bIsRMBDown = true;
}

void AHexplorePlayerController::RMBCompleted(const FInputActionValue& InputActionValue)
{
	if (!bIsRMBDragging)
	{
		// Tap: set combat target if clicking on a valid target
		if (AHexploreCharacter* PlayerCharacter = GetPawn<AHexploreCharacter>())
		{
			FHitResult CursorHit;
			GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

			if (CursorHit.bBlockingHit)
			{
				AActor* HitActor = CursorHit.GetActor();
				if (Cast<ITargetInterface>(HitActor) && HitActor != PlayerCharacter->GetCombatTarget())
				{
					PlayerCharacter->SetCombatTarget(HitActor);
				}
			}
		}
	}

	bIsRMBDown = false;
	bIsRMBDragging = false;
	UpdateRotationMode();
}

void AHexplorePlayerController::MouseDelta(const FInputActionValue& InputActionValue)
{
	const FVector2D MouseAxis = InputActionValue.Get<FVector2D>();

	if (bIsLMBDown)
	{
		bIsLMBDragging = true;
		AddYawInput(MouseAxis.X * CameraSensitivity);
		AddPitchInput(MouseAxis.Y * CameraSensitivity);
	}

	if (bIsRMBDown)
	{
		if (!bIsRMBDragging)
		{
			bIsRMBDragging = true;
			UpdateRotationMode();
		}
		AddYawInput(MouseAxis.X * CameraSensitivity);
		AddPitchInput(MouseAxis.Y * CameraSensitivity);
	}
}

void AHexplorePlayerController::ScrollWheel(const FInputActionValue& InputActionValue)
{
	if (USpringArmComponent* SpringArm = GetSpringArm())
	{
		const float ScrollValue = InputActionValue.Get<float>();
		SpringArm->TargetArmLength = FMath::Clamp(
			SpringArm->TargetArmLength - ScrollValue * ZoomStep,
			MinZoomDistance,
			MaxZoomDistance);
	}
}

/*
 * Targeting
 */

void AHexplorePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor != nullptr) LastActor->UnHighlightActor();
		if (ThisActor != nullptr) ThisActor->HighlightActor();
	}
}

/*
 * Abilities
 */

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