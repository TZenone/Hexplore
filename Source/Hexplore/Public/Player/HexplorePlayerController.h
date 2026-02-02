// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexplorePlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexplorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHexplorePlayerController();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HexploreContext;
};
