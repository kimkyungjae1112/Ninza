// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NIUpperAttackNotify.generated.h"

UCLASS()
class NINZA_API UNIUpperAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UNIUpperAttackNotify();


};
