// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NIHUDWidget.generated.h"

class UNIHpBarWidget;

UCLASS()
class NINZA_API UNIHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UNIHUDWidget(const FObjectInitializer& ObjectInitializer);

	void SetMaxHp(float InMaxHp);
	void UpdateHpBar(float InCurrentHp);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNIHpBarWidget> HpBar;
};
