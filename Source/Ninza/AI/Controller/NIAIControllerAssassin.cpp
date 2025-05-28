// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/NIAIControllerAssassin.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "Ninza.h"

ANIAIControllerAssassin::ANIAIControllerAssassin()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTDataRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BehaviorTree/BT_Assassin.BT_Assassin'"));
	if (BTDataRef.Object)
	{
		BTData = BTDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/Blackboard/BB_Assassin.BB_Assassin'"));
	if (BBDataRef.Object)
	{
		BBData = BBDataRef.Object;
	}

	// Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2200.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(2.f);

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*DamageConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ANIAIControllerAssassin::OnPerceptionUpdated);

	SetPerceptionComponent(*PerceptionComp);

	TeamID = FGenericTeamId(1);
}

void ANIAIControllerAssassin::UpperHitSense(bool bIsUpperedHit)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsUpperHit"), bIsUpperedHit);
}

void ANIAIControllerAssassin::BeginPlay()
{
	Super::BeginPlay();
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsUpperHit"), false);
}

void ANIAIControllerAssassin::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
	if (Attitude == ETeamAttitude::Hostile)
	{
		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Sight);
		HandleSenseSight(Actor, Stimulus);

		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Damage);
		HandleSenseDamage(Actor, Stimulus);
	}
}

FAIStimulus ANIAIControllerAssassin::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	PerceptionComp->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	//AI 가 지금 감지할 수 있는 감각 Sight, Damage
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_None:
		break;
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;

			return ResultStimulus;
		}
	}

	return ResultStimulus;
}

void ANIAIControllerAssassin::HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus)
{
	if (AIStimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}

void ANIAIControllerAssassin::HandleSenseDamage(AActor* Actor, const FAIStimulus& AIStimulus)
{
	if (AIStimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), true);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsHit"), false);
	}
}
