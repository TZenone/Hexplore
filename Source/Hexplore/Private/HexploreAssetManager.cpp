// Copyright Tristan ZENONE.


#include "HexploreAssetManager.h"

#include "HexploreGameplayTags.h"

UHexploreAssetManager& UHexploreAssetManager::Get()
{
	check(GEngine);
	
	UHexploreAssetManager* HexploreAssetManager = Cast<UHexploreAssetManager>(GEngine->AssetManager);
	return *HexploreAssetManager;
}

void UHexploreAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FHexploreGameplayTags::InitializeNativeGameplayTags();
}
