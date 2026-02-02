// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "HexploreCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreCharacter : public AHexploreCharacterBase
{
	GENERATED_BODY()

public:
	
	AHexploreCharacter();

	UFUNCTION(BlueprintCallable)
	void SetCameraMode(bool bAim);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Rotation")
	float MoveRotationRateYaw = 720.f;

	UPROPERTY(EditDefaultsOnly, Category="Rotation")
	float AimRotationRateYaw = 1080.f;
};
