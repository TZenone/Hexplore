// Copyright Tristan ZENONE.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/HexploreWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UHexploreUserWidget;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxStaminaChangedSignature, float, NewMaxStamina);

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Message = FText();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UHexploreUserWidget> MessageWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Image = nullptr;
};


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HEXPLORE_API UOverlayWidgetController : public UHexploreWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnStaminaChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxStaminaChangedSignature OnMaxStaminaChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void StaminaChanged(const FOnAttributeChangeData& Data) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& Data) const;
};
