// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NIStatBaseComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZero)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChanged, float /* Current Hp */)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINZA_API UNIStatBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNIStatBaseComponent();

public:
	void ApplyDamage(float InDamage);
	
	FOnHpZero OnHpZero;
	FOnHpChanged OnHpChanged;


protected:
	virtual void BeginPlay() override;

	void SetHp(float NewHp);

protected:
	float MaxHp;
	float CurrentHp;
		
};
