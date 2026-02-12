// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "Character/HexploreCharacterBase.h"
#include "HexploreCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HEXPLORE_API AHexploreCharacter : public AHexploreCharacterBase
{
	GENERATED_BODY()

public:
	AHexploreCharacter();
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/* Combat Interface */
	/* End Combat Interface */

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:

	void AttackSpeedChanged(const FOnAttributeChangeData& Data);
	void TryAutoAttack() const;
	
	virtual void OnEngaged(AActor* Target) override;

	virtual void OnDisengaged(AActor* Target) override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnAttackSpeedChanged;
	
	FTimerHandle AutoAttackTimerHandle;

private:
	virtual void InitAbilityActorInfo() override;
};
