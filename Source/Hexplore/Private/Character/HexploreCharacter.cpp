// Copyright Tristan ZENONE.


#include "Character/HexploreCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AHexploreCharacter::AHexploreCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->bOrientRotationToMovement = false;
}

void AHexploreCharacter::SetCameraMode(bool bAim)
{
	// Aim: face controller/cursor
	bUseControllerRotationYaw = bAim;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		// Move: face movement direction
		MoveComp->bOrientRotationToMovement = !bAim;

		MoveComp->RotationRate = FRotator(0.f, bAim ? AimRotationRateYaw : MoveRotationRateYaw, 0.f);
	}
}
