// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NIHpBarWidget.generated.h"

class UProgressBar;

UCLASS()
class NINZA_API UNIHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UNIHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHp(float InMaxHp) { MaxHp = InMaxHp; }
	void UpdateHpBar(float InCurrentHp);

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProgressBar> HpBar;

	float MaxHp;
};
