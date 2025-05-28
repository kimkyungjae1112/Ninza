// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/NIEnemyAttackComponent.h"
#include "Interface/NIClassIdentifierInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Ninza.h"

UNIEnemyAttackComponent::UNIEnemyAttackComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MoveTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/NIAnimMontageDataTable.NIAnimMontageDataTable'"));
	if (MoveTableRef.Object)
	{
		MoveTable = MoveTableRef.Object;
	}
}

void UNIEnemyAttackComponent::BeginAttackOnWalk()
{
	Anim->Montage_Play(GetAttackOnWalkMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UNIEnemyAttackComponent::EndAttackOnWalk);
	Anim->Montage_SetEndDelegate(MontageEnd, GetAttackOnWalkMontage());
}

void UNIEnemyAttackComponent::EndAttackOnWalk(UAnimMontage* Target, bool IsProperlyEnd)
{
	NI_LOG(LogNinza, Display, TEXT("Attack End"));
	AttackOnWalkFinish.ExecuteIfBound();
}

void UNIEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Anim = Character->GetMesh()->GetAnimInstance();
		ensure(Anim);

		INIClassIdentifierInterface* CII = Cast<INIClassIdentifierInterface>(Character);
		if (CII)
		{
			MontageData = *MoveTable->FindRow<FNIAnimMontageDataTable>(CII->GetClassName(), FString());
		}
	}
}

UAnimMontage* UNIEnemyAttackComponent::GetAttackOnWalkMontage() const
{
	if (MontageData.AttackOnWalkMontage.IsPending())
	{
		MontageData.AttackOnWalkMontage.LoadSynchronous();
	}
	return MontageData.AttackOnWalkMontage.Get();
}


