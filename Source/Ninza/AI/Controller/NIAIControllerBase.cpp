// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/NIAIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

ANIAIControllerBase::ANIAIControllerBase()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
}

void ANIAIControllerBase::Run()
{
	UBlackboardComponent* BBComp = Blackboard.Get();
	if (UseBlackboard(BBData, BBComp))
	{
		bool RunResult = RunBehaviorTree(BTData);
	}
}

void ANIAIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void ANIAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Run();
}

FGenericTeamId ANIAIControllerBase::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type ANIAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		if (OtherTeamId == TeamID)
		{
			return ETeamAttitude::Friendly; // 아군
		}
		else if (OtherTeamId != TeamID)
		{
			return ETeamAttitude::Hostile; // 적군
		}
		else
		{
			return ETeamAttitude::Neutral;
		}
	}
	return ETeamAttitude::Neutral; // 중립
}
