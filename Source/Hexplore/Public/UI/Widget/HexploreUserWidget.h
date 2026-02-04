// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HexploreUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
