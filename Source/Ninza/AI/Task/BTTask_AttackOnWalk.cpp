// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_AttackOnWalk.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/NIAIInterface.h"
#include "Ninza.h"

EBTNodeResult::Type UBTTask_AttackOnWalk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return EBTNodeResult::Failed;

	INIAIInterface* Interface = Cast<INIAIInterface>(Owner);
	if (Interface == nullptr) return EBTNodeResult::Failed;

	FAttackOnWalkFinished AttackOnWalkFinished;
	AttackOnWalkFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			OwnerComp.GetBlackboardComponent()->SetValueAsName(TEXT("LastAction"), TEXT("Attack"));
			PenaltyCount++;
		}
	);

	Interface->SetAIAttackOnWalkDelegate(AttackOnWalkFinished);
	Interface->AttackOnWalkByAI();

	return EBTNodeResult::InProgress;
}

UBTTask_AttackOnWalk::UBTTask_AttackOnWalk()
{
	NodeName = TEXT("Assassin Attack");
}

float UBTTask_AttackOnWalk::GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return -1.f;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return -1.f;

	INIAIInterface* Interface = Cast<INIAIInterface>(Owner);
	if (Interface == nullptr) return -1.f;

	FName LastAction = OwnerComp.GetBlackboardComponent()->GetValueAsName(TEXT("LastAction"));

	float FinalUtility{ 0.f };
	float Dist = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation());
	float DistanceRatio = FMath::Clamp((Interface->GetAttakRange() - Dist) / Interface->GetAttakRange(), 0.f, 1.f);
	FinalUtility = FMath::Pow(DistanceRatio, 3) * 1000.f;
	float Penalty = (LastAction == TEXT("Attack")) ? 100.f * PenaltyCount : 1.0f;
	NI_LOG(LogNinza, Display, TEXT("Attack Utility : %f"), FinalUtility - Penalty);

	return FinalUtility - Penalty;
}
