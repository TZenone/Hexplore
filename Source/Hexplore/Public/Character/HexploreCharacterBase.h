// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HexploreCharacterBase.generated.h"

UCLASS(Abstract)
class HEXPLORE_API AHexploreCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AHexploreCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
