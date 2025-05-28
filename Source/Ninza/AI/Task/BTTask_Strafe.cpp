// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Strafe.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Interface/NIAIInterface.h"
#include "Ninza.h"

EBTNodeResult::Type UBTTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector ViewDirection = (Target->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
	FVector RightDirection = FVector::CrossProduct(ViewDirection, FVector::UpVector);
	FVector StrafeDirection = FMath::RandBool() ? RightDirection : -RightDirection;

	FVector Start = Owner->GetActorLocation();
	FVector Dest = Start + StrafeDirection * 500.f;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Owner->GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation ProjectedDest;
	EPathFollowingRequestResult::Type Result;
	if (NavSystem->ProjectPointToNavigation(Dest, ProjectedDest))
	{
		Result = OwnerComp.GetAIOwner()->MoveToLocation(ProjectedDest, 5.f);
		PenaltyCount++;
	}
	
	return EBTNodeResult::InProgress;
}

UBTTask_Strafe::UBTTask_Strafe()
{
	NodeName = TEXT("Strafe");
	bNotifyTick = true;
}

float UBTTask_Strafe::GetUtilityScore(const UBehaviorTreeComponent& OwnerComp) const
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return -1.f;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return -1.f;

	INIAIInterface* Interface = Cast<INIAIInterface>(Owner);
	if (Interface == nullptr) return -1.f;

	FName LastAction = OwnerComp.GetBlackboardComponent()->GetValueAsName(TEXT("LastAction"));

	float AttackRange = Interface->GetAttakRange();
	float Dist = FVector::Dist(Target->GetActorLocation(), Owner->GetActorLocation());

	float OptimalDist = AttackRange;
	float RangeTolerance = 200.f;

	float DistanceDiff = FMath::Abs(Dist - OptimalDist);
	float DistanceRatio = 1.f - FMath::Clamp(DistanceDiff / RangeTolerance, 0.f, 1.f);  // 1 → 0

	float FinalUtility = DistanceRatio * 100.f;
	float Penalty = (LastAction == TEXT("Strafe")) ? 100.f * PenaltyCount : 0.f;
	float Result = FinalUtility - Penalty;
	NI_LOG(LogNinza, Display, TEXT("Strafe Utility: %f (Dist: %f)"), Result, Dist);

	return Result;
}

void UBTTask_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetAIOwner()->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsName(TEXT("LastAction"), TEXT("Strafe"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
