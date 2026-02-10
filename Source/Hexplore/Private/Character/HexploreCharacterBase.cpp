// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"

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
	if (UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetAttackSpeedAttribute()).AddUObject(this, &AHexploreCharacterBase::AttackSpeedChanged);
	}
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

// Function to Update the attack speed in case it gets modified During Combat
void AHexploreCharacterBase::AttackSpeedChanged(const FOnAttributeChangeData& Data)
{
	// If we're not in combat, return.
	if (!bIsInCombat) return;

	// Else, Clear the running timer, and update it with the new AttackSpeed Attribute value received from the delegate. 
	GetWorldTimerManager().ClearTimer(AutoAttackTimerHandle);
	TryAutoAttack();
	GetWorldTimerManager().SetTimer(AutoAttackTimerHandle,this, &AHexploreCharacterBase::TryAutoAttack, Data.NewValue, true);
	UE_LOG(LogTemp, Warning, TEXT("Starting Timer in AttackSpeedChanged & Firing AutoAttack"));
}

void AHexploreCharacterBase::TryAutoAttack() const
{
	AbilitySystemComponent->TryActivateAbilityByClass(BasicAttackClass);
}

void AHexploreCharacterBase::SetCombatTarget(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Target of [%s] is [%s]"), *GetName(),  *Target->GetName());
	CombatTarget = Target;
	bIsInCombat = true;
	
	if (UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet))
	{
		TryAutoAttack();
		GetWorldTimerManager().SetTimer(AutoAttackTimerHandle,this, &AHexploreCharacterBase::TryAutoAttack, AS->GetAttackSpeed(), true);
		UE_LOG(LogTemp, Warning, TEXT("Starting Timer in SetCombatTarget & Firing AutoAttack"));
	}
	
}

void AHexploreCharacterBase::ClearCombatTarget()
{
	if (CombatTarget != nullptr)
	{
		CombatTarget = nullptr;
		bIsInCombat = false;
	}
}

AActor* AHexploreCharacterBase::GetCombatTarget() const
{
	return CombatTarget;
}

