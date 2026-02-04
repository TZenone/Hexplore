// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HexploreHUD.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UOverlayWidgetController;
class UHexploreUserWidget;

struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UHexploreUserWidget> OverlayWidget;

	UFUNCTION(BlueprintCallable)
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHexploreUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
