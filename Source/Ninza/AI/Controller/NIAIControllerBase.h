// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "NIAIControllerBase.generated.h"

class UBehaviorTree;
class UBlackboardData;
class UAIPerceptionComponent;

UENUM()
enum class EAIPerceptionSense : uint8
{
	EPS_None = 0,
	EPS_Sight,
	EPS_Damage
};

UCLASS()
class NINZA_API ANIAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	ANIAIControllerBase();

	void Run();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere, Category = "BT")
	TObjectPtr<UBehaviorTree> BTData;

	UPROPERTY(VisibleAnywhere, Category = "BB")
	TObjectPtr<UBlackboardData> BBData;

	FGenericTeamId TeamID;
};
