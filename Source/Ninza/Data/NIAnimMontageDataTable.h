
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NIAnimMontageDataTable.generated.h"

USTRUCT(BlueprintType)
struct FNIAnimMontageDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> AttackOnIdleMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> AttackOnWalkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> AttackOnRunMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> AttackOnFocusingMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> AttackOnJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TSoftObjectPtr<UAnimMontage>> HitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TSoftObjectPtr<UAnimMontage>> LargeHitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> UpperedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> UpperedHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> StandUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TSoftObjectPtr<UAnimMontage> RushAttackMontage;
};