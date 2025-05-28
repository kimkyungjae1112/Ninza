// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHit.generated.h"

UCLASS()
class NINZA_API UBTDecorator_IsHit : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsHit();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
