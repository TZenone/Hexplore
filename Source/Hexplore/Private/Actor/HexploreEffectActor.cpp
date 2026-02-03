// Copyright Tristan ZENONE.


#include "Actor/HexploreEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/HexploreAttributeSet.h"
#include "Components/SphereComponent.h"

AHexploreEffectActor::AHexploreEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void AHexploreEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a GameplayEffect. For now using const_cast as a hack.
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UHexploreAttributeSet* HexploreAS = Cast<UHexploreAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UHexploreAttributeSet::StaticClass()));

		// oof
		UHexploreAttributeSet* MutableHexploreAS = const_cast<UHexploreAttributeSet*>(HexploreAS);
		MutableHexploreAS->SetHealth(HexploreAS->GetHealth() + 25.f);
		Destroy();
	}
}

void AHexploreEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherOverlappedComponent, int32 OtherBodyIndex)
{
	
}


void AHexploreEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AHexploreEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AHexploreEffectActor::EndOverlap);
}

