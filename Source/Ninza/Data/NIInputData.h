// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NIInputData.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class NINZA_API UNIInputData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Idle;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Walk;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Run;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Jump;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Attacking;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Defensing;

	UPROPERTY(EditAnywhere, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Focusing;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Run;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Crouch;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnIdle;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnCrouch;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnWalk;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnRun;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnJump;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_AttackOnFocusing;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Attacking;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Defense;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_FocusOn;
	UPROPERTY(EditAnywhere, Category = "Input Action")
	TObjectPtr<UInputAction> IA_RushAttack;
};
