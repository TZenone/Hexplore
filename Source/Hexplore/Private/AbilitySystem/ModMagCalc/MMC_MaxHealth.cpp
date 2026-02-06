// Copyright Tristan ZENONE.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/HexploreAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UHexploreAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Parameters that we have to pass to 'GetCapturedAttributeMagnitude()' in order to capture the Attribute we're interested in.
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	
	// Capture the attribute
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	// Clamping
	Vigor = FMath::Max<float>(Vigor, 0.f);

	/*
	 * If we had a level, we would get the level from the Interface implementing the level. This is just as an example,
	 * we don't have levels in our game.
	 * We would uncomment the following lines if we had a player level.
	 * We would then use 'Custom Calculation Class in our GameplayEffect Modifier
	 * Magnitude dropdown, selecting 'MMC_MaxHealth'.
	*/
	
	// ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	// const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	const int32 PlayerLevel = 1;
	
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
