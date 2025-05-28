// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/NIPlayerSwordNotifyState.h"
#include "Interface/NISwordInterface.h"
#include "Engine/DamageEvents.h"

UNIPlayerSwordNotifyState::UNIPlayerSwordNotifyState()
{
}

void UNIPlayerSwordNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

}

void UNIPlayerSwordNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp->GetOwner())
	{
		MakeLineTrace(MeshComp->GetOwner());
	}
}

void UNIPlayerSwordNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HitActors.Empty();
}

void UNIPlayerSwordNotifyState::MakeLineTrace(AActor* Character)
{
	INISwordInterface* Interface = Cast<INISwordInterface>(Character);
	if (Interface)
	{
		USkeletalMeshComponent* SwordMeshComp = Interface->GetSwordComp();
		if (SwordMeshComp)
		{
			FVector SwordBoneStart = SwordMeshComp->GetSocketLocation(TEXT("SwordBoneStart"));
			FVector SwordBoneEnd = SwordMeshComp->GetSocketLocation(TEXT("SwordBoneEnd"));

			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, Character);
			UWorld* World = Character->GetWorld();
			check(World);

			bool bHit = World->LineTraceSingleByChannel(
				HitResult,
				SwordBoneStart,
				SwordBoneEnd,
				ECC_GameTraceChannel2,
				Params
			);

			DrawDebugLine(World, SwordBoneStart, SwordBoneEnd, FColor::Red, false, 3.f);

			if (bHit && !HitActors.Contains(HitResult.GetActor()))
			{
				HitActors.Add(HitResult.GetActor());

				if (HitResult.GetActor())
				{
					APawn* Instigator = Cast<APawn>(Character);
					FDamageEvent DamageEvent;
					HitResult.GetActor()->TakeDamage
					(
						Interface->GetSwordDamage()[Interface->GetPlayerState()],
						DamageEvent, 
						Instigator->GetController(), 
						Character
					);

					DrawDebugSphere(World, HitResult.ImpactPoint, 15.f, 32, FColor::Green, false, 3.f);
				}
			}
		}
	}
}
