// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

AHexploreCharacterBase::AHexploreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHexploreCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

