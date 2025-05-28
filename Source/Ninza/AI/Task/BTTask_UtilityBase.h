// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Interface/NIAIUtilityInterface.h"
#include "BTTask_UtilityBase.generated.h"

UCLASS()
class NINZA_API UBTTask_UtilityBase 
	: public UBTTaskNode
	, public INIAIUtilityInterface
{
	GENERATED_BODY()
	
public:
	virtual float GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const override;

protected:
	int32 PenaltyCount{ 0 };
};
