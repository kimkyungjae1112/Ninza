// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NIHpBarWidget.h"
#include "Components/ProgressBar.h"

UNIHpBarWidget::UNIHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNIHpBarWidget::UpdateHpBar(float InCurrentHp)
{
	HpBar->SetPercent(InCurrentHp / MaxHp);
}

void UNIHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HpBar")));
	ensure(HpBar);
}
