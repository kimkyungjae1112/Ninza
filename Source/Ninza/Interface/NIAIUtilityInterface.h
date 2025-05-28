// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NIAIUtilityInterface.generated.h"

class UBehaviorTreeComponent;

UINTERFACE(MinimalAPI)
class UNIAIUtilityInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INIAIUtilityInterface
{
	GENERATED_BODY()

public:
	virtual float GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const = 0;
};
