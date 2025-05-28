// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Composite/BTComposite_UtilitySelector.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTNode.h"
#include "Interface/NIAIUtilityInterface.h"
#include "../Task/BTTask_UtilityBase.h"

int32 UBTComposite_UtilitySelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	Super::GetNextChildHandler(SearchData, PrevChild, LastResult);

    float BestScore = -500000000.f;
    int32 BestIndex = BTSpecialChild::ReturnToParent;

    for (int32 i = 0; i < GetChildrenNum(); ++i)
    {
        UBTTask_UtilityBase* Task = Cast<UBTTask_UtilityBase>(Children[i].ChildTask);
        if (!Task) continue;

        INIAIUtilityInterface* UtilityTask = Cast<UBTTask_UtilityBase>(Task);
        if (!UtilityTask) continue;

        float Score = UtilityTask->GetUtilityScore(SearchData.OwnerComp);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestIndex = i;
        }
    }

    return BestIndex;
}
