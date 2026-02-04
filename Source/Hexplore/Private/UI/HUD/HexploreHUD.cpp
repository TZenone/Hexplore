// Copyright Tristan ZENONE.


#include "UI/HUD/HexploreHUD.h"

#include "UI/Widget/HexploreUserWidget.h"


void AHexploreHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
