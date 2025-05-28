// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/NIAttackComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/NIAnimInstance.h"
#include "Data/NIComboAttackData.h"
#include "Engine/OverlapResult.h"
#include "Interface/NIClassIdentifierInterface.h"
#include "Interface/NIOutlineInterface.h"
#include "Interface/NICameraInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "Ninza.h"

UNIAttackComponent::UNIAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> MontageTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/NIAnimMontageDataTable.NIAnimMontageDataTable'"));
	if (MontageTableRef.Object)
	{
		MontageTable = MontageTableRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UNIComboAttackData> ComboDataRef(TEXT("/Script/Ninza.NIComboAttackData'/Game/Data/Combo/DA_ComboAttack.DA_ComboAttack'"));
	if (ComboDataRef.Object)
	{
		ComboData = ComboDataRef.Object;
	}
}

void UNIAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsFocuesd)
	{
		FRotator TargetRot = (FocusedTarget->GetActorLocation() - Player->GetActorLocation()).Rotation();
		GetPlayerController()->SetControlRotation(TargetRot + FRotator(-25.f, 0.f, 0.f));
	}
}

void UNIAttackComponent::AttackOnIdle()
{
	if (CurrentComboOnIdle == 0)
	{
		BeginAttackOnIdle();
		return;
	}

	if (!ComboTimerHandleOnIdle.IsValid())
	{
		HasNextComboAttackOnIdle = false;
	}
	else
	{
		HasNextComboAttackOnIdle = true;
	}
}

void UNIAttackComponent::AttackOnWalk()
{
	if (CurrentComboOnWalk == 0)
	{
		BeginAttackOnWalk();
		return;
	}

	if (!ComboTimerHandleOnWalk.IsValid())
	{
		HasNextComboAttackOnWalk = false;
	}
	else
	{
		HasNextComboAttackOnWalk = true;
	}
}

void UNIAttackComponent::AttackOnRun()
{
	if (CurrentComboOnRun == 0)
	{
		BeginAttackOnRun();
		return;
	}

	if (!ComboTimerHandleOnRun.IsValid())
	{
		HasNextComboAttackOnRun = false;
	}
	else
	{
		HasNextComboAttackOnRun = true;
	}
}

void UNIAttackComponent::AttackOnFocusing()
{
	if (CurrentComboOnFocusing == 0)
	{
		BeginAttackOnFocusing();
		return;
	}

	if (!ComboTimerHandleOnFocusing.IsValid())
	{
		HasNextComboAttackOnFocusing = false;
	}
	else
	{
		HasNextComboAttackOnFocusing = true;
	}
}

void UNIAttackComponent::RushAttack()
{
	if (bIsFocuesd)
	{
		//	SetRushAttackMotionWarpSet();
		Anim->Montage_Play(GetMontageRushAttack());
		NI_LOG(LogNinza, Display, TEXT("Rush Attack"));

		FOnMontageEnded MontageEnd;
		MontageEnd.BindUObject(this, &UNIAttackComponent::EndRushAttack);
		Anim->Montage_SetEndDelegate(MontageEnd, GetMontageRushAttack());
	}
}

void UNIAttackComponent::Defense()
{
	UNIAnimInstance* NIAnim = Cast<UNIAnimInstance>(Anim);
	if (NIAnim)
	{
		NIAnim->bIsDefense = true;
	}
}

void UNIAttackComponent::UnDefense()
{
	UNIAnimInstance* NIAnim = Cast<UNIAnimInstance>(Anim);
	if (NIAnim)
	{
		NIAnim->bIsDefense = false;
	}
}

void UNIAttackComponent::FocusingOn()
{
	TArray<FOverlapResult> OverlapResults;
	FVector Origin = Player->GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, Player);

	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults, Origin, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(FocusDistance), Param);
	if (bHit)
	{
		float MinDist = FocusDistance + 1.f;
		for (const auto& Result : OverlapResults)
		{
			AActor* Target = Result.GetActor();
			float Dist = FVector::Dist(Origin, Target->GetActorLocation());
			if (Dist < MinDist)
			{
				Dist = MinDist;
				FocusedTarget = Target;
			};
		}

		bIsFocuesd = true;
		GetPlayerController()->SetIgnoreLookInput(true);

		INIOutlineInterface* Interface = Cast<INIOutlineInterface>(FocusedTarget);
		if (Interface) Interface->SetOverlayMaterial();
	}
}

void UNIAttackComponent::UnFocusingOn()
{
	bIsFocuesd = false;
	GetPlayerController()->SetIgnoreLookInput(false);
	INIOutlineInterface* Interface = Cast<INIOutlineInterface>(FocusedTarget);
	if (Interface) Interface->SetUnOverlayMaterial();
	FocusedTarget = nullptr;
}

void UNIAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if (Player)
	{
		Anim = Player->GetMesh()->GetAnimInstance();
		ensure(Anim);
	}

	INIClassIdentifierInterface* CII = Cast<INIClassIdentifierInterface>(Player);
	if (CII)
	{
		MontageData = *MontageTable->FindRow<FNIAnimMontageDataTable>(CII->GetClassName(), FString());
	}
}

APlayerController* UNIAttackComponent::GetPlayerController() const
{
	return CastChecked<APlayerController>(Player->GetController());
}

void UNIAttackComponent::BeginAttackOnIdle()
{
	CurrentComboOnIdle = 1;
	Anim->Montage_Play(GetMontageAttackOnIdle());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UNIAttackComponent::EndAttackOnIdle);
	Anim->Montage_SetEndDelegate(MontageEnd, GetMontageAttackOnIdle());

	ComboTimerHandleOnIdle.Invalidate();
	SetComboCheckTimerOnIdle();
}

void UNIAttackComponent::EndAttackOnIdle(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentComboOnIdle != 0);
	CurrentComboOnIdle = 0;
	OnEndAttack.Broadcast(EBehaviorState::EIdle);
}

void UNIAttackComponent::SetComboCheckTimerOnIdle()
{
	int32 ComboIndex = CurrentComboOnIdle - 1;
	ensure(ComboData->AttackOnIdleCombo.EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboData->AttackOnIdleCombo.EffectiveFrameCount[ComboIndex] / ComboData->AttackOnIdleCombo.FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandleOnIdle, this, &UNIAttackComponent::ComboCheckOnIdle, ComboEffectiveTime, false);
	}
}

void UNIAttackComponent::ComboCheckOnIdle()
{
	if (HasNextComboAttackOnIdle)
	{
		CurrentComboOnIdle = FMath::Clamp(CurrentComboOnIdle + 1, 0, ComboData->AttackOnIdleCombo.MaxCombo);
		FName SectionName = *FString::Printf(TEXT("%s%d"), *ComboData->AttackOnIdleCombo.SectionName, CurrentComboOnIdle);
		Anim->Montage_JumpToSection(SectionName, MontageData.AttackOnIdleMontage.Get());

		SetComboCheckTimerOnIdle();
		HasNextComboAttackOnIdle = false;
	}
}

void UNIAttackComponent::BeginAttackOnWalk()
{
	CurrentComboOnWalk = 1;
	Anim->Montage_Play(GetMontageAttackOnWalk());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UNIAttackComponent::EndAttackOnWalk);
	Anim->Montage_SetEndDelegate(MontageEnd, GetMontageAttackOnWalk());

	ComboTimerHandleOnWalk.Invalidate();
	SetComboCheckTimerOnWalk();

}

void UNIAttackComponent::EndAttackOnWalk(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentComboOnWalk != 0);
	CurrentComboOnWalk = 0;
	OnEndAttack.Broadcast(EBehaviorState::EIdle);
}

void UNIAttackComponent::SetComboCheckTimerOnWalk()
{
	int32 ComboIndex = CurrentComboOnWalk - 1;
	ensure(ComboData->AttackOnWalkCombo.EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboData->AttackOnWalkCombo.EffectiveFrameCount[ComboIndex] / ComboData->AttackOnWalkCombo.FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandleOnWalk, this, &UNIAttackComponent::ComboCheckOnWalk, ComboEffectiveTime, false);
	}
}

void UNIAttackComponent::ComboCheckOnWalk()
{
	ComboTimerHandleOnWalk.Invalidate();
	if (HasNextComboAttackOnWalk)
	{
		CurrentComboOnWalk = FMath::Clamp(CurrentComboOnWalk + 1, 1, ComboData->AttackOnWalkCombo.MaxCombo);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboData->AttackOnWalkCombo.SectionName, CurrentComboOnWalk);
		Anim->Montage_JumpToSection(NextSection, MontageData.AttackOnWalkMontage.Get());

		SetComboCheckTimerOnWalk();
		HasNextComboAttackOnWalk = false;
	}
}

void UNIAttackComponent::BeginAttackOnRun()
{
	CurrentComboOnRun = 1;
	Anim->Montage_Play(GetMontageAttackOnRun());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UNIAttackComponent::EndAttackOnRun);
	Anim->Montage_SetEndDelegate(MontageEnd, GetMontageAttackOnRun());

	ComboTimerHandleOnRun.Invalidate();
	SetComboCheckTimerOnRun();
}

void UNIAttackComponent::EndAttackOnRun(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentComboOnRun != 0);
	CurrentComboOnRun = 0;
	OnEndAttack.Broadcast(EBehaviorState::EIdle);
}

void UNIAttackComponent::SetComboCheckTimerOnRun()
{
	int32 ComboIndex = CurrentComboOnRun - 1;
	ensure(ComboData->AttackOnRunCombo.EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboData->AttackOnRunCombo.EffectiveFrameCount[ComboIndex] / ComboData->AttackOnRunCombo.FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandleOnRun, this, &UNIAttackComponent::ComboCheckOnRun, ComboEffectiveTime, false);
	}
}

void UNIAttackComponent::ComboCheckOnRun()
{
	if (HasNextComboAttackOnRun)
	{
		CurrentComboOnRun = FMath::Clamp(CurrentComboOnRun + 1, 1, ComboData->AttackOnRunCombo.MaxCombo);
		FName SectionName = *FString::Printf(TEXT("%s%d"), *ComboData->AttackOnRunCombo.SectionName, CurrentComboOnRun);
		Anim->Montage_JumpToSection(SectionName, MontageData.AttackOnRunMontage.Get());

		SetComboCheckTimerOnRun();
		HasNextComboAttackOnRun = false;
	}
}

void UNIAttackComponent::BeginAttackOnFocusing()
{
	CurrentComboOnFocusing = 1;
	Anim->Montage_Play(GetMontageAttackOnFocusing());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UNIAttackComponent::EndAttackOnFocusing);
	Anim->Montage_SetEndDelegate(MontageEnd, GetMontageAttackOnFocusing());

	ComboTimerHandleOnFocusing.Invalidate();
	SetComboCheckTimerOnFocusing();
}

void UNIAttackComponent::EndAttackOnFocusing(UAnimMontage* Target, bool IsProperlyEnded)
{
	ensure(CurrentComboOnFocusing != 0);
	CurrentComboOnFocusing = 0;
	OnEndAttack.Broadcast(EBehaviorState::EFocusing);
}

void UNIAttackComponent::SetComboCheckTimerOnFocusing()
{
	int32 ComboIndex = CurrentComboOnFocusing - 1;
	ensure(ComboData->AttackOnFocusingCombo.EffectiveFrameCount.IsValidIndex(ComboIndex));

	float ComboEffectiveTime = (ComboData->AttackOnFocusingCombo.EffectiveFrameCount[ComboIndex] / ComboData->AttackOnFocusingCombo.FrameRate);
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandleOnFocusing, this, &UNIAttackComponent::ComboCheckOnFocusing, ComboEffectiveTime, false);
	}
}

void UNIAttackComponent::ComboCheckOnFocusing()
{
	if (HasNextComboAttackOnFocusing)
	{
		CurrentComboOnFocusing = FMath::Clamp(CurrentComboOnFocusing + 1, 1, ComboData->AttackOnFocusingCombo.MaxCombo);
		FName SectionName = *FString::Printf(TEXT("%s%d"), *ComboData->AttackOnFocusingCombo.SectionName, CurrentComboOnFocusing);
		Anim->Montage_JumpToSection(SectionName, GetMontageAttackOnFocusing());

		SetComboCheckTimerOnFocusing();
		HasNextComboAttackOnFocusing = false;
	}
}

void UNIAttackComponent::EndRushAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnEndAttack.Broadcast(EBehaviorState::EIdle);
}

UAnimMontage* UNIAttackComponent::GetMontageAttackOnIdle() const
{
	if (MontageData.AttackOnIdleMontage.IsPending())
	{
		MontageData.AttackOnIdleMontage.LoadSynchronous();
	}
	return MontageData.AttackOnIdleMontage.Get();
}

UAnimMontage* UNIAttackComponent::GetMontageAttackOnWalk() const
{
	if (MontageData.AttackOnWalkMontage.IsPending())
	{
		MontageData.AttackOnWalkMontage.LoadSynchronous();
	}
	return MontageData.AttackOnWalkMontage.Get();
}

UAnimMontage* UNIAttackComponent::GetMontageAttackOnRun() const
{
	if (MontageData.AttackOnRunMontage.IsPending())
	{
		MontageData.AttackOnRunMontage.LoadSynchronous();
	}
	return MontageData.AttackOnRunMontage.Get();
}

UAnimMontage* UNIAttackComponent::GetMontageAttackOnFocusing() const
{
	if (MontageData.AttackOnFocusingMontage.IsPending())
	{
		MontageData.AttackOnFocusingMontage.LoadSynchronous();
	}
	return MontageData.AttackOnFocusingMontage.Get();
}

UAnimMontage* UNIAttackComponent::GetMontageRushAttack() const
{
	if (MontageData.RushAttackMontage.IsPending())
	{
		MontageData.RushAttackMontage.LoadSynchronous();
	}
	return MontageData.RushAttackMontage.Get();
}

UMotionWarpingComponent* UNIAttackComponent::GetMotionWarpComp() const
{
	return Player->GetComponentByClass<UMotionWarpingComponent>();
}

void UNIAttackComponent::SetRushAttackMotionWarpSet() const
{
	FVector OriginLoc = Player->GetActorLocation();
	FVector TargetLoc = FocusedTarget->GetActorLocation();
	float Dist = FVector::Dist(OriginLoc, TargetLoc);
	
	FVector Target = OriginLoc + TargetLoc * (Dist - 200.f);
	GetMotionWarpComp()->AddOrUpdateWarpTargetFromLocation(TEXT("Rush"), Target);
}



