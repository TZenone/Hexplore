// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HexploreAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API UHexploreAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UHexploreAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
