// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexploreEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class HEXPLORE_API AHexploreEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHexploreEffectActor();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> EffectClass;
};
