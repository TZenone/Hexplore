// Copyright Tristan ZENONE.


#include "Player/HexplorePlayerController.h"
#include "EnhancedInputSubsystems.h"

AHexplorePlayerController::AHexplorePlayerController()
{
	bReplicates = true;
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
