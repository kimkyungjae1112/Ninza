// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NIAirToFloorAttackNotify.generated.h"

UCLASS()
class NINZA_API UNIAirToFloorAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UNIAirToFloorAttackNotify();

};
