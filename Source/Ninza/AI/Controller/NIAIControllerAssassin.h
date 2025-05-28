// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/NIAIControllerBase.h"
#include "NIAIControllerAssassin.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class NINZA_API ANIAIControllerAssassin : public ANIAIControllerBase
{
	GENERATED_BODY()
	
public:
	ANIAIControllerAssassin();

	void UpperHitSense(bool bIsUpperedHit);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	void HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus);
	void HandleSenseDamage(AActor* Actor, const FAIStimulus& AIStimulus);

private:
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
};
