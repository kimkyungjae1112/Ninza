// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NIPlayerSwordNotifyState.generated.h"

class ACharacter;

UCLASS()
class NINZA_API UNIPlayerSwordNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UNIPlayerSwordNotifyState();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

	void MakeLineTrace(AActor* Character);

private:
	UPROPERTY()
	TSet<AActor*> HitActors;
};
