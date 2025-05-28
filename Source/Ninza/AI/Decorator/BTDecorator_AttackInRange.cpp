// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/NIAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* Owner = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return false;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return false;

	INIAIInterface* Interface = Cast<INIAIInterface>(Owner);
	if (Interface == nullptr) return false;

	float Dist = FVector::Dist(Owner->GetActorLocation(), Target->GetActorLocation());
	return Dist < Interface->GetAttakRange();
}
