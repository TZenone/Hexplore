// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HexploreAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreAIController : public AAIController
{
	GENERATED_BODY()

	AHexploreAIController();

public:
	
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
