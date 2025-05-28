// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NIHUDWidget.h"
#include "UI/NIHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/NIHUDInterface.h"

UNIHUDWidget::UNIHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNIHUDWidget::SetMaxHp(float InMaxHp)
{
	HpBar->SetMaxHp(InMaxHp);
}

void UNIHUDWidget::UpdateHpBar(float InCurrentHp)
{
	HpBar->UpdateHpBar(InCurrentHp);
}

void UNIHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UNIHpBarWidget>(GetWidgetFromName(TEXT("WBP_HpBar")));

	INIHUDInterface* Interface = Cast<INIHUDInterface>(GetOwningPlayerPawn());
	if (Interface)
	{
		Interface->SetHUD(this);
	}
}
