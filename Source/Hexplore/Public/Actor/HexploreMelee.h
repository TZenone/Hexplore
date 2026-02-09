// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexploreMelee.generated.h"

UCLASS()
class HEXPLORE_API AHexploreMelee : public AActor
{
	GENERATED_BODY()
	
public:	
	AHexploreMelee();

protected:
	virtual void BeginPlay() override;
	
};
