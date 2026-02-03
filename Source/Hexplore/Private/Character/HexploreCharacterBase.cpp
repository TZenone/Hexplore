// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

AHexploreCharacterBase::AHexploreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AHexploreCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHexploreCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

