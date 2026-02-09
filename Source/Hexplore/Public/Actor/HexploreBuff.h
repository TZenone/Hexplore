// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexploreBuff.generated.h"

UCLASS()
class HEXPLORE_API AHexploreBuff : public AActor
{
	GENERATED_BODY()
	
public:	
	AHexploreBuff();

protected:
	virtual void BeginPlay() override;

private:
	
};
