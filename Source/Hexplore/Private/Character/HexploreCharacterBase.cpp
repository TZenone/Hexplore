// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "Components/SphereComponent.h"

AHexploreCharacterBase::AHexploreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EngagementRange = CreateDefaultSubobject<USphereComponent>("EngagementRange");
	EngagementRange->SetupAttachment(GetMesh());
	EngagementRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EngagementRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	EngagementRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EngagementRange->SetSphereRadius(EngagementRadius);
}

UAbilitySystemComponent* AHexploreCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHexploreCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EngagementRange->OnComponentBeginOverlap.AddDynamic(this, &AHexploreCharacterBase::OnEngagementRangeBeginOverlap);
	EngagementRange->OnComponentEndOverlap.AddDynamic(this, &AHexploreCharacterBase::OnEngagementRangeEndOverlap);
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

void AHexploreCharacterBase::EngageTarget(AActor* TargetToEngage)
{
	EngagedTarget = TargetToEngage;
	EngagedTargetAdded.Broadcast(TargetToEngage);
}

void AHexploreCharacterBase::DisengageTarget(AActor* TargetToDisengage)
{
	EngagedTarget = nullptr;
	EngagedTargetRemoved.Broadcast(TargetToDisengage);
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

void AHexploreCharacterBase::OnEngagementRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Entered [%s]'s Engagement Range."), *OtherActor->GetName(), *GetName());
		if (OtherActor == CombatTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Entered [%s]'s Engagement Range and Started Melee combat."), *OtherActor->GetName(), *GetName());
			EngageTarget(OtherActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Exited [%s]'s Engagement Range but wasn't the Combat Target."), *OtherActor->GetName(), *GetName());
		}
	}
}

void AHexploreCharacterBase::OnEngagementRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Exited [%s]'s Engagement Range."), *OtherActor->GetName(), *GetName());
		if (OtherActor == CombatTarget)
		{
			DisengageTarget(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("[%s] Exited [%s]'s Engagement Range and Triggered and Opportunity Attack."), *OtherActor->GetName(), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Exited [%s]'s Engagement Range but wasn't the Combat Target."), *OtherActor->GetName(), *GetName());
		}
	}
}

void AHexploreCharacterBase::SetCombatTarget(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Target of [%s] is [%s]"), *GetName(),  *Target->GetName());
	CombatTarget = Target;
	bIsInCombat = true;

	// Engage Target if already in Range
	if (GetDistanceTo(Target) < EngagementRadius)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target was already in Range, Engaging [%s]"), *Target->GetName());
		EngageTarget(Target);
	}
	
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

