// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_UtilityBase.h"
#include "BTTask_Strafe.generated.h"

UCLASS()
class NINZA_API UBTTask_Strafe 
	: public UBTTask_UtilityBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTask_Strafe();

	virtual float GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
