// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HexploreHUD.generated.h"

class UHexploreUserWidget;
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

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHexploreUserWidget> OverlayWidgetClass;
};
