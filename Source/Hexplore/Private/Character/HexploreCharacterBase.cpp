// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"

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

FVector AHexploreCharacterBase::GetBuffSocketLocation()
{
	return GetMesh()->GetSocketLocation(BuffSocketName);
}

void AHexploreCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AHexploreCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	ApplyEffectToSelf(DefaultRegenAttributes, 1.f);
}

void AHexploreCharacterBase::AddCharacterAbilities()
{
	UHexploreAbilitySystemComponent* HexploreASC = CastChecked<UHexploreAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	HexploreASC->AddCharacterAbilities(StartupAbilities);
}

void AHexploreCharacterBase::SetCurrentTarget(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Target of [%s] is [%s]"), *GetName(),  *Target->GetName());
	CurrentTarget = Target;
	bIsInCombat = true;
	
	GetAbilitySystemComponent()->TryActivateAbilityByClass(BasicAttackClass);
}

AActor* AHexploreCharacterBase::GetCurrentTarget() const
{
	if (CurrentTarget == nullptr) return nullptr;
	return CurrentTarget;
}

