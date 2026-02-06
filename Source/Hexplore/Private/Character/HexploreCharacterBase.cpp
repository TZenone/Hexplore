// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"

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

void AHexploreCharacterBase::InitAbilityActorInfo()
{
	
}

void AHexploreCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

