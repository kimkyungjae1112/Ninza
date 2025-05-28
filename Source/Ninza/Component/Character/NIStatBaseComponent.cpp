// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/NIStatBaseComponent.h"

UNIStatBaseComponent::UNIStatBaseComponent()
{
	MaxHp = 1000.f;
	CurrentHp = MaxHp;
}

void UNIStatBaseComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = InDamage;

	SetHp(PrevHp - InDamage);
	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}


void UNIStatBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UNIStatBaseComponent::SetHp(float NewHp)
{
	CurrentHp = NewHp;
	OnHpChanged.Broadcast(CurrentHp);
}



