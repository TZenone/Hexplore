// Copyright Tristan ZENONE.


#include "AbilitySystem/HexploreAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UHexploreAttributeSet::UHexploreAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(50.f);
	InitMaxStamina(100.f);
	InitStamina(50.f);
}

void UHexploreAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Endurance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, ArmorPen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, Block, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, CritMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHexploreAttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);
	
}

void UHexploreAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UHexploreAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UHexploreAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
}

void UHexploreAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Health, OldHealth);
}

void UHexploreAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, MaxHealth, OldMaxHealth);
}

void UHexploreAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Stamina, OldStamina);
}

void UHexploreAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, MaxStamina, OldMaxStamina);
}

void UHexploreAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Strength, OldStrength);
}

void UHexploreAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Dexterity, OldDexterity);
}

void UHexploreAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Vigor, OldVigor);
}

void UHexploreAttributeSet::OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Endurance, OldEndurance);
}

void UHexploreAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Armor, OldArmor);
}

void UHexploreAttributeSet::OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, ArmorPen, OldArmorPen);
}

void UHexploreAttributeSet::OnRep_Block(const FGameplayAttributeData& OldBlock) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, Block, OldBlock);
}

void UHexploreAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, CritChance, OldCritChance);
}

void UHexploreAttributeSet::OnRep_CritMultiplier(const FGameplayAttributeData& OldCritMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, CritMultiplier, OldCritMultiplier);
}

void UHexploreAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, HealthRegen, OldHealthRegen);
}

void UHexploreAttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHexploreAttributeSet, StaminaRegen, OldStaminaRegen);
}
