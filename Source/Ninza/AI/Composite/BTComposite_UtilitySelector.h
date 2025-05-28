// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BTComposite_UtilitySelector.generated.h"

UCLASS(Blueprintable)
class NINZA_API UBTComposite_UtilitySelector : public UBTComposite_Selector
{
	GENERATED_BODY()
	
	virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;
};
