// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NICameraData.generated.h"

UCLASS()
class NINZA_API UNICameraData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Pawn")
	bool bUseControllerRotationYaw;

	UPROPERTY(EditAnywhere, Category = "Character Movement")
	bool bOrientRotationToMovement;

	UPROPERTY(EditAnywhere, Category = "Spring Arm")
	bool bUsePawnControlRotation;
};
