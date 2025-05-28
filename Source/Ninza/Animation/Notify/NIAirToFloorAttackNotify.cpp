// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/NIAirToFloorAttackNotify.h"
#include "Component/Character/NIDamagedComponent.h"
#include "Ninza.h"

void UNIAirToFloorAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

    AActor* Player = MeshComp->GetOwner();
    if (Player)
    {
        FVector Start = Player->GetActorLocation();
        FVector Forward = Player->GetActorForwardVector();

        FVector Diagonal = (Forward + FVector(0.f, 0.f, -0.5f)).GetSafeNormal();
        FVector End = Start + Diagonal * 500.f;

        FVector HalfSize(300.f, 300.f, 300.f); 
        FCollisionShape CollisionShape = FCollisionShape::MakeBox(HalfSize);
        FQuat Rotation = FRotationMatrix::MakeFromX(Diagonal).ToQuat();

        FCollisionQueryParams Params(NAME_None, false, Player);
        TArray<FHitResult> HitResults;

        DrawDebugBox(Player->GetWorld(), Start, HalfSize, Rotation, FColor::Green, false, 2.f);
        DrawDebugBox(Player->GetWorld(), End, HalfSize, Rotation, FColor::Red, false, 2.f);
        DrawDebugLine(Player->GetWorld(), Start, End, FColor::Blue, false, 2.f, 0, 2.f);
        NI_LOG(LogNinza, Display, TEXT("AirToFloor Attack"));

        bool bHit = Player->GetWorld()->SweepMultiByChannel(
            HitResults,
            Start,
            End,
            Rotation,
            ECC_GameTraceChannel2,
            CollisionShape,
            Params
        );

        if (bHit)
        {
            for (const FHitResult& Hit : HitResults)
            {
                NI_LOG(LogNinza, Display, TEXT("Hit: %s"), *Hit.GetActor()->GetName());

                AActor* Target = Hit.GetActor();
                if (Target)
                {
                    UNIDamagedComponent* DamagedComp = Target->GetComponentByClass<UNIDamagedComponent>();
                    if (DamagedComp)
                    {
                        DamagedComp->BeginUpperedHit(Player);
                    }
                }
            }
        }
    }
}

UNIAirToFloorAttackNotify::UNIAirToFloorAttackNotify()
{
}
