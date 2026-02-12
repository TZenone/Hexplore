// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexplorePlayerController.generated.h"

class USpringArmComponent;
class UHexploreInputConfig;
class UInputMappingContext;
class UInputAction;
class ITargetInterface;
class UHexploreAbilitySystemComponent;

struct FGameplayTag;
struct FInputActionValue;

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

	/*
	 * Input Actions
	 */

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HexploreContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LMBAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> RMBAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseDeltaAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ScrollWheelAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UHexploreInputConfig> InputConfig;

	/*
	 * Input Handlers
	 */

	void Move(const FInputActionValue& InputActionValue);
	void SprintStart(const FInputActionValue& InputActionValue);
	void SprintEnd(const FInputActionValue& InputActionValue);
	void LMBStarted(const FInputActionValue& InputActionValue);
	void LMBCompleted(const FInputActionValue& InputActionValue);
	void RMBStarted(const FInputActionValue& InputActionValue);
	void RMBCompleted(const FInputActionValue& InputActionValue);
	void MouseDelta(const FInputActionValue& InputActionValue);
	void ScrollWheel(const FInputActionValue& InputActionValue);

	/*
	 * Camera
	 */

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomStep = 100.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoomDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoomDistance = 800.f;

	TObjectPtr<USpringArmComponent> CachedSpringArm;
	USpringArmComponent* GetSpringArm();

	/*
	 * Mouse State
	 */

	bool bIsLMBDown = false;
	bool bIsRMBDown = false;
	bool bIsLMBDragging = false;
	bool bIsRMBDragging = false;
	bool bIsSprinting = false;

	/*
	 * Targeting
	 */

	void CursorTrace();
	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	/*
	 * Character Movement State
	 */

	void UpdateRotationMode();

	/*
	 * Abilities
	 */

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UHexploreAbilitySystemComponent> HexploreAbilitySystemComponent;
	UHexploreAbilitySystemComponent* GetASC();
};