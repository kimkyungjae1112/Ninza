// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/NIUpperAttackNotify.h"
#include "Component/Character/NIDamagedComponent.h"
#include "GameFramework/Character.h"
#include "Engine/OverlapResult.h"
#include "Ninza.h"

void UNIUpperAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Player = Cast<ACharacter>(MeshComp->GetOwner());
	if (Player)
	{
		FVector Origin = Player->GetActorLocation();
		FCollisionQueryParams Params(NAME_None, false, Player);
		TArray<FOverlapResult> OverlapResults;

		bool bHit = Player->GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat(), ECC_GameTraceChannel2, FCollisionShape::MakeSphere(400.f), Params);
		DrawDebugSphere(Player->GetWorld(), Origin, 400.f, 32, FColor::Red, false, 3.f);
		if (bHit)
		{
			NI_LOG(LogNinza, Display, TEXT("Hit"));
			AActor* Target{ nullptr };
			for (auto Result : OverlapResults) Target = Result.GetActor();
			NI_LOG(LogNinza, Display, TEXT("Hit Actor Name : %s"), *Target->GetActorNameOrLabel());

			if (Target)
			{
				UNIDamagedComponent* DamagedComp = Target->GetComponentByClass<UNIDamagedComponent>();
				if (DamagedComp)
				{
					DamagedComp->BeginUppered(Player);
					Player->LaunchCharacter(FVector(0.f, 0.f, 500.f), false, true);
				}
			}
		}
	}



	// MeshComp->GetOwner()->GetComponentByClass<UNIDamagedComponent>();
	// 몽타주 실행
	// MotionWarping 설정
	// Z 축의 최대 높이에 도달 -> 중력 끄기
	// 캐릭터의 마지막 공격 이후 쓰러지는 몽타주 실행
	// 캐릭터가 바닥에 닿으면 일어나는 모션 실행 


	// 가능할까?!
}

UNIUpperAttackNotify::UNIUpperAttackNotify()
{
}
