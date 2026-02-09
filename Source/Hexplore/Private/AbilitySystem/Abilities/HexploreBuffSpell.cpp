// Copyright Tristan ZENONE.


#include "AbilitySystem/Abilities/HexploreBuffSpell.h"

#include "Actor/HexploreBuff.h"
#include "Interaction/CombatInterface.h"

void UHexploreBuffSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetBuffSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
	
		AHexploreBuff* Projectile = GetWorld()->SpawnActorDeferred<AHexploreBuff>(
			BuffClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		// TODO: Give the buff a GameplayEffectSpec for applying a buff.

		Projectile->FinishSpawning(SpawnTransform);
	}
}
