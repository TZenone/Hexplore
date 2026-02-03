// Copyright Tristan ZENONE.


#include "Character/HexploreEnemy.h"

#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"

AHexploreEnemy::AHexploreEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UHexploreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UHexploreAttributeSet>("AttributeSet");
}

void AHexploreEnemy::HighlightActor()
{
	bHighlighted = true;
}

void AHexploreEnemy::UnHighlightActor()
{
	bHighlighted = false;
}
