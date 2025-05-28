// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Character/NICharacterPlayerState.h"
#include "NISwordInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNISwordInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INISwordInterface
{
	GENERATED_BODY()

public:
	virtual USkeletalMeshComponent* GetSwordComp() const = 0;
	virtual EBehaviorState GetPlayerState() const = 0;
	virtual TMap<EBehaviorState, float> GetSwordDamage() const = 0;
};

