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

	TObjectPtr<USpringArmComponent> CachedSpringArm;
	USpringArmComponent* GetSpringArm();

	bool bIsLMBDown = false;
	bool bIsRMBDown = false;
	bool bIsLMBDragging = false;
	bool bIsRMBDragging = false;
	float SensitivityMultiplier = 0.5f;

	void Move(const FInputActionValue& InputActionValue);
	void SprintStart(const FInputActionValue& InputActionValue);
	void SprintEnd(const FInputActionValue& InputActionValue);

	void LMBStarted(const FInputActionValue& InputActionValue);
	void LMBCompleted(const FInputActionValue& InputActionValue);
	void RMBStarted(const FInputActionValue& InputActionValue);
	void RMBCompleted(const FInputActionValue& InputActionValue);
	void MouseDelta(const FInputActionValue& InputActionValue);
	void ScrollWheel(const FInputActionValue& InputActionValue);
	
	void CursorTrace();

	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UHexploreInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "ASC")
	TObjectPtr<UHexploreAbilitySystemComponent> HexploreAbilitySystemComponent;

	UHexploreAbilitySystemComponent* GetASC();
};
