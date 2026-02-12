// Copyright Tristan ZENONE.


#include "Character/HexploreCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"
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
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AHexploreCharacterBase::OnDisengaged(AActor* Target)
{
	// GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AHexploreCharacterBase::OnEngagementRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	UE_LOG(LogTemp, Warning, TEXT("[%s] isnt the same actor, continuing."),  *GetName());
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Entered [%s]'s Engagement Range."), *OtherActor->GetName(), *GetName());
		UE_LOG(LogTemp, Warning, TEXT("[%s]'s Current Combat Target is [%s]."), *GetName(), CombatTarget != nullptr ? *CombatTarget->GetName() : TEXT("not set."));
		if (OtherActor == CombatTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Entered [%s]'s Engagement Range and Started Melee combat."), *OtherActor->GetName(), *GetName());
			EngageTarget(OtherActor);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] Entered [%s]'s Engagement Range but wasn't the Combat Target."), *OtherActor->GetName(), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Doesnt pass the CombatInterface check."), *GetName());
	}
}

void AHexploreCharacterBase::OnEngagementRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Exited [%s]'s Engagement Range."), *OtherActor->GetName(), *GetName());
		if (OtherActor == CombatTarget)
		{
			DisengageTarget(OtherActor);
			AbilitySystemComponent->TryActivateAbilityByClass(OpportunityActionClass);
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

