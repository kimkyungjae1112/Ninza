// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_UtilityBase.h"
#include "BTTask_AttackOnWalk.generated.h"

UCLASS()
class NINZA_API UBTTask_AttackOnWalk : public UBTTask_UtilityBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTask_AttackOnWalk();

	/* INIAIUtilityInterface Implement */
	virtual float GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const override;

};
