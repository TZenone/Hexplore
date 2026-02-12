// Copyright Tristan ZENONE.


#include "Character/HexploreEnemy.h"

#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "AI/HexploreAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/HexploreUserWidget.h"

AHexploreEnemy::AHexploreEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UHexploreAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UHexploreAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AHexploreEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	HexploreAIController = Cast<AHexploreAIController>(NewController);

	HexploreAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	HexploreAIController->RunBehaviorTree(BehaviorTree);
	
	HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), false);
	
	// TODO: No Character class right now, but will implement later
	// HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Melee);

	AddCharacterAbilities();
}

void AHexploreEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (UHexploreUserWidget* HexploreUserWidget = Cast<UHexploreUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		HexploreUserWidget->SetWidgetController(this);
	}

	EngagedTargetAdded.AddDynamic(this, &AHexploreEnemy::OnEngaged);
	EngagedTargetRemoved.AddDynamic(this, &AHexploreEnemy::OnDisengaged);
	
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
	
	Super::InitAbilityActorInfo();
}

void AHexploreEnemy::OnEngaged(AActor* Target)
{
	Super::OnEngaged(Target);
	
	HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsEngaged"), true);
}

void AHexploreEnemy::OnDisengaged(AActor* Target)
{
	Super::OnDisengaged(Target);
	
	HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsEngaged"), false);
}

void AHexploreEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	
	// TODO: Implement real speed based on a move speed secondary Attribute
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : 300.f;
	
	HexploreAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AHexploreEnemy::HighlightActor()
{
	bHighlighted = true;
}

void AHexploreEnemy::UnHighlightActor()
{
	bHighlighted = false;
}
