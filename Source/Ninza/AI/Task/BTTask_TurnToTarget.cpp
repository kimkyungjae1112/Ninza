// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/NIAIInterface.h"

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* Owner = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return EBTNodeResult::Failed;

	INIAIInterface* Interface = Cast<INIAIInterface>(Owner);
	if (Interface == nullptr) return EBTNodeResult::Failed;

	float TurnSpeed = Interface->GetTurnSpeed();
	FVector LookVector = Target->GetActorLocation() - Owner->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));

	return EBTNodeResult::Succeeded;
}

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}
