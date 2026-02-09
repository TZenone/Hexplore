// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexplorePlayerController.generated.h"

struct FGameplayTag;
class UHexploreInputConfig;
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

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	void Move(const FInputActionValue& InputActionValue);
	void SprintStart(const FInputActionValue& InputActionValue);
	void SprintEnd(const FInputActionValue& InputActionValue);
	
	void CursorTrace();

	TScriptInterface<ITargetInterface> LastActor;
	TScriptInterface<ITargetInterface> ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UHexploreInputConfig> InputConfig;
};
