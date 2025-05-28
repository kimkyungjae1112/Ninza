// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NIComboAttackData.generated.h"

USTRUCT()
struct FAttackOnIdleCombo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Frame")
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	FString SectionName;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Combo")
	int32 MaxCombo;
};

USTRUCT()
struct FAttackOnWalkCombo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Frame")
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	FString SectionName;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Combo")
	int32 MaxCombo;
};

USTRUCT()
struct FAttackOnRunCombo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Frame")
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	FString SectionName;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Combo")
	int32 MaxCombo;
};

USTRUCT()
struct FAttackOnFocusingCombo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Frame")
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	FString SectionName;

	UPROPERTY(EditAnywhere, Category = "Rate")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "Combo")
	int32 MaxCombo;
};


UCLASS()
class NINZA_API UNIComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Attack On Idle")
	FAttackOnWalkCombo AttackOnIdleCombo;

	UPROPERTY(EditAnywhere, Category = "Attack On Walk")
	FAttackOnWalkCombo AttackOnWalkCombo;

	UPROPERTY(EditAnywhere, Category = "Attack On Run")
	FAttackOnWalkCombo AttackOnRunCombo;

	UPROPERTY(EditAnywhere, Category = "Attack On Focusing")
	FAttackOnWalkCombo AttackOnFocusingCombo;
};
