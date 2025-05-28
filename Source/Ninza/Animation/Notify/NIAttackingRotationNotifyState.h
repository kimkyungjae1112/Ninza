// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NIAttackingRotationNotifyState.generated.h"

class ACharacter;

UCLASS()
class NINZA_API UNIAttackingRotationNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UNIAttackingRotationNotifyState();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> Player;
};
