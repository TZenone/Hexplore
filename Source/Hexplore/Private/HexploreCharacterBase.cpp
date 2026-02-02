// Copyright Tristan ZENONE.


#include "HexploreCharacterBase.h"

// Sets default values
AHexploreCharacterBase::AHexploreCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexploreCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHexploreCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHexploreCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

