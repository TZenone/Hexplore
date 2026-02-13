// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHexploreCharacterBase::AHexploreCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EngagementRange = CreateDefaultSubobject<USphereComponent>("EngagementRange");
	EngagementRange->SetupAttachment(GetRootComponent());
	EngagementRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	EngagementRange->SetCollisionResponseToAllChannels(ECR_Ignore);
	EngagementRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	EngagementRange->SetSphereRadius(EngagementRadius);
}

UAbilitySystemComponent* AHexploreCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AHexploreCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AHexploreCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	EngagementRange->OnComponentBeginOverlap.AddDynamic(this, &AHexploreCharacterBase::OnEngagementRangeBeginOverlap);
	EngagementRange->OnComponentEndOverlap.AddDynamic(this, &AHexploreCharacterBase::OnEngagementRangeEndOverlap);
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

void AHexploreCharacterBase::OnEngaged(AActor* Target)
{
	GetCharacterMovement()->MaxWalkSpeed = EngagementMovementSpeed;
}

void AHexploreCharacterBase::OnDisengaged(AActor* Target)
{
	// GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AHexploreCharacterBase::OnEngagementRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		if (OtherActor == CombatTarget)
		{
			EngageTarget(OtherActor);
		}
	}
}

void AHexploreCharacterBase::OnEngagementRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		if (OtherActor == CombatTarget)
		{
			DisengageTarget(OtherActor);
			AbilitySystemComponent->TryActivateAbilityByClass(OpportunityActionClass);
		}
	}
}

void AHexploreCharacterBase::SetCombatTarget(AActor* Target)
{
	CombatTarget = Target;
	bIsInCombat = true;

	// Engage Target if already in Range
	if (GetDistanceTo(Target) < EngagementRadius)
	{
		EngageTarget(Target);
	}
}

void AHexploreCharacterBase::ClearCombatTarget()
{
	if (CombatTarget != nullptr)
	{
		EngagedTarget = nullptr;
		CombatTarget = nullptr;
		bIsInCombat = false;
	}
}

AActor* AHexploreCharacterBase::GetCombatTarget() const
{
	return CombatTarget;
}

void AHexploreCharacterBase::OpportunityAction()
{
	
}

void AHexploreCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	
	MulticastHandleDeath();
}

void AHexploreCharacterBase::MulticastHandleDeath_Implementation()
{
	/*Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);*/

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::NoCollision));
}

