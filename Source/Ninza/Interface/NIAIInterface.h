// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NIAIInterface.generated.h"

class AAIController;

DECLARE_DELEGATE(FAttackOnWalkFinished)

UINTERFACE(MinimalAPI)
class UNIAIInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INIAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetTurnSpeed() const = 0;
	virtual float GetAttakRange() const = 0;

	virtual void SetAIAttackOnWalkDelegate(const FAttackOnWalkFinished& InAttackOnWalkFinished) = 0;


	virtual void AttackOnWalkByAI() = 0;

	virtual AAIController* GetAIController() const = 0;
};
