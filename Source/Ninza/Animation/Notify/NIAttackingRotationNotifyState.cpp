// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/NIAttackingRotationNotifyState.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/Character/NIMovementComponent.h"
#include "Ninza.h"

UNIAttackingRotationNotifyState::UNIAttackingRotationNotifyState()
{
}

void UNIAttackingRotationNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Player = Cast<ACharacter>(MeshComp->GetOwner());
	if (Player)
	{
		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void UNIAttackingRotationNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	UNIMovementComponent* MoveComp = MeshComp->GetOwner()->GetComponentByClass<UNIMovementComponent>();
	if (MoveComp && Player)
	{
		FVector Forward = Player->GetActorForwardVector() * MoveComp->GetMoveInput().X;
		FVector Right = Player->GetActorRightVector() * MoveComp->GetMoveInput().Y;
		FVector FinalVec = (Forward + Right).GetSafeNormal();

		if (!FinalVec.IsNearlyZero())
		{
			FRotator FinalRot = FinalVec.Rotation();
			FRotator FinalYawRot(0.f, FinalRot.Yaw, 0.f);
			MeshComp->GetOwner()->SetActorRotation(FMath::RInterpTo(MeshComp->GetOwner()->GetActorRotation(), FinalYawRot, FrameDeltaTime, 5.f));
		}
	}
}

void UNIAttackingRotationNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Player)
	{
		Player->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}