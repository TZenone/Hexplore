// Copyright Tristan ZENONE.


#include "Character/HexploreCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/HexploreAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/HexplorePlayerController.h"
#include "Player/HexplorePlayerState.h"
#include "UI/HUD/HexploreHUD.h"

AHexploreCharacter::AHexploreCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AHexploreCharacter::BeginPlay()
{
	Super::BeginPlay();

	EngagedTargetAdded.AddDynamic(this, &AHexploreCharacter::OnEngaged);
	EngagedTargetRemoved.AddDynamic(this, &AHexploreCharacter::OnDisengaged);
}

void AHexploreCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AHexploreCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability actor info for the client
	InitAbilityActorInfo();
}

// Function to Update the attack speed in case it gets modified During Combat
void AHexploreCharacter::AttackSpeedChanged(const FOnAttributeChangeData& Data)
{
	// If we're not in combat, return.
	if (!AutoAttackTimerHandle.IsValid()) return;

	// Else, Clear the running timer, and update it with the new AttackSpeed Attribute value received from the delegate. 
	GetWorldTimerManager().ClearTimer(AutoAttackTimerHandle);
	TryAutoAttack();
	GetWorldTimerManager().SetTimer(AutoAttackTimerHandle,this, &AHexploreCharacter::TryAutoAttack, Data.NewValue, true);
	UE_LOG(LogTemp, Warning, TEXT("Starting Timer in AttackSpeedChanged & Firing AutoAttack"));
}

void AHexploreCharacter::TryAutoAttack() const
{
	AbilitySystemComponent->TryActivateAbilityByClass(BasicAttackClass);
}

void AHexploreCharacter::OnEngaged(AActor* Target)
{
	Super::OnEngaged(Target);
	
	if (UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] is engaging [%s]'s Engagement Range."), *GetName(), *Target->GetName());
		
		TryAutoAttack();
		GetWorldTimerManager().SetTimer(AutoAttackTimerHandle,this, &AHexploreCharacter::TryAutoAttack, AS->GetAttackSpeed(), true);
		UE_LOG(LogTemp, Error, TEXT("Starting Timer in SetCombatTarget & Firing AutoAttack"));
	}
}

void AHexploreCharacter::OnDisengaged(AActor* Target)
{
	Super::OnDisengaged(Target);
	
	GetWorldTimerManager().ClearTimer(AutoAttackTimerHandle);
}

void AHexploreCharacter::InitAbilityActorInfo()
{
	AHexplorePlayerState* PS =  GetPlayerState<AHexplorePlayerState>();
	check(PS);
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	Cast<UHexploreAbilitySystemComponent>(PS->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSet = PS->GetAttributeSet();

	if (UHexploreAttributeSet* AS = CastChecked<UHexploreAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetAttackSpeedAttribute()).AddUObject(this, &AHexploreCharacter::AttackSpeedChanged);
	}

	if (AHexplorePlayerController* PC = Cast<AHexplorePlayerController>(GetController()))
	{
		if (AHexploreHUD* HUD = Cast<AHexploreHUD>(PC->GetHUD()))
		{
			HUD->InitOverlay(PC, PS, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
	
	Super::InitAbilityActorInfo();
}
