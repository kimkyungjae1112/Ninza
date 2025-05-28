// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/NIDamagedComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/NIClassIdentifierInterface.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "MotionWarpingComponent.h"
#include "Engine/DamageEvents.h"
#include "Ninza.h"

UNIDamagedComponent::UNIDamagedComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MoveTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/NIAnimMontageDataTable.NIAnimMontageDataTable'"));
	if (MoveTableRef.Object)
	{
		MoveTable = MoveTableRef.Object;
	}
}

void UNIDamagedComponent::Damaged()
{
	Anim->Montage_Play(GetLargeHitMontage());
}

void UNIDamagedComponent::BeginUppered(AActor* Causer)
{
	bIsUppered = true;
	OnFinishedUpperHit.Broadcast(bIsUppered);
	APawn* PawnCauser = Cast<APawn>(Causer);
	Player->TakeDamage(100.f, FDamageEvent(), PawnCauser->GetController(), Causer);
	Player->LaunchCharacter(FVector(0.f, 0.f, 700.f), false, true);

}

void UNIDamagedComponent::EndUppered()
{
	bIsUppered = false;
	OnFinishedUpperHit.Broadcast(bIsUppered);
}

void UNIDamagedComponent::BeginUpperedHit(AActor* Causer)
{
	bIsUpperedHit = true;
	bIsUppered = false;
	Anim->Montage_Play(GetUpperedHitMontage());
}

void UNIDamagedComponent::BeginStandUp()
{
	if (bIsUpperedHit)
	{
		Anim->Montage_Play(GetStandUpMontage());

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &UNIDamagedComponent::EndStandUp);
		Anim->Montage_SetEndDelegate(MontageEnd, GetStandUpMontage());
	}
}

void UNIDamagedComponent::EndStandUp(UAnimMontage* Target, bool IsProperlyEnd)
{
	bIsUpperedHit = false;
	OnFinishedUpperHit.Broadcast(bIsUppered);
}


void UNIDamagedComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if (Player)
	{
		Anim = Player->GetMesh()->GetAnimInstance();
		ensure(Anim);

		INIClassIdentifierInterface* CII = Cast<INIClassIdentifierInterface>(Player);
		if (CII)
		{
			MontageData = *MoveTable->FindRow<FNIAnimMontageDataTable>(CII->GetClassName(), FString());
		}
	}
}

UAnimMontage* UNIDamagedComponent::GetLargeHitMontage() const
{
	if (MontageData.LargeHitMontages[0].IsPending())
	{
		MontageData.LargeHitMontages[0].LoadSynchronous();
	}
	return MontageData.LargeHitMontages[0].Get();
}

UAnimMontage* UNIDamagedComponent::GetUpperedMontage() const
{
	if (MontageData.UpperedMontage.IsPending())
	{
		MontageData.UpperedMontage.LoadSynchronous();
	}
	return MontageData.UpperedMontage.Get();
}

UAnimMontage* UNIDamagedComponent::GetUpperedHitMontage() const
{
	if (MontageData.UpperedHitMontage.IsPending())
	{
		MontageData.UpperedHitMontage.LoadSynchronous();
	}
	return MontageData.UpperedHitMontage.Get();
}

UAnimMontage* UNIDamagedComponent::GetStandUpMontage() const
{
	if (MontageData.StandUpMontage.IsPending())
	{
		MontageData.StandUpMontage.LoadSynchronous();
	}
	return MontageData.StandUpMontage.Get();
}

UMotionWarpingComponent* UNIDamagedComponent::GetMotionWarpComp() const
{
	return Player->GetComponentByClass<UMotionWarpingComponent>();
}

void UNIDamagedComponent::UpperedMotionWarpSet() const
{
	//Launch 로 구현하는게 더 자연스러운거 같다....
	FVector Origin = Player->GetActorLocation();
	FVector Target = Origin + FVector(0.f, 0.f, 500.f);
	GetMotionWarpComp()->AddOrUpdateWarpTargetFromLocation(TEXT("Uppered"), Target);
}



