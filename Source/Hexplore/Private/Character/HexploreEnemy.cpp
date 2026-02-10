// Copyright Tristan ZENONE.


#include "Character/HexploreEnemy.h"

#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/HexploreUserWidget.h"

AHexploreEnemy::AHexploreEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UHexploreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UHexploreAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AHexploreEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UHexploreUserWidget* HexploreUserWidget = Cast<UHexploreUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		HexploreUserWidget->SetWidgetController(this);
	}
	
	if (const UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}

void AHexploreEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UHexploreAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	InitializeDefaultAttributes();
}

void AHexploreEnemy::HighlightActor()
{
	bHighlighted = true;
}

void AHexploreEnemy::UnHighlightActor()
{
	bHighlighted = false;
}
