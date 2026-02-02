// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexplorePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ITargetInterface;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexplorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHexplorePlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HexploreContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> RotateCameraAction;

	void Move(const FInputActionValue& InputActionValue);

	void RMBPressed(const FInputActionValue& Value);
	void RMBReleased(const FInputActionValue& Value);
	void UpdateControlRotationToCursor(float DeltaTime);
	
	void CursorTrace();

	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	bool bRotateCameraHeld = false;

	UPROPERTY(EditDefaultsOnly, Category="Camera|Aim")
	float AimYawDegreesPerSecond = 90.f;

	UPROPERTY(EditDefaultsOnly, Category="Camera|Aim")
	float CenterDeadZoneRadiusPx = 90.f; 
};
