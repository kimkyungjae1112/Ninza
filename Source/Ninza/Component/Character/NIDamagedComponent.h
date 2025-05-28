// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/NIAnimMontageDataTable.h"
#include "NIDamagedComponent.generated.h"

class UAnimMontage;
class UAnimInstance;
class UMotionWarpingComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishedUpperHit, bool)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINZA_API UNIDamagedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNIDamagedComponent();

	void Damaged();

	void BeginUppered(AActor* Causer);
	void EndUppered();
	void BeginUpperedHit(AActor* Causer);
	void BeginStandUp();
	void EndStandUp(UAnimMontage* Target, bool IsProperlyEnd);

	FORCEINLINE bool GetIsUppered() const { return bIsUppered; }
	FORCEINLINE bool GetIsUpperedHit() const { return bIsUpperedHit; }

public:
	FOnFinishedUpperHit OnFinishedUpperHit;

protected:
	virtual void BeginPlay() override;

// Owner
private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Player;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<UAnimInstance> Anim;

// Animation Data
private:
	UAnimMontage* GetLargeHitMontage() const;
	UAnimMontage* GetUpperedMontage() const;
	UAnimMontage* GetUpperedHitMontage() const;
	UAnimMontage* GetStandUpMontage() const;


	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UDataTable> MoveTable;

	FNIAnimMontageDataTable MontageData;

// MotionWarping
private:
	UMotionWarpingComponent* GetMotionWarpComp() const;

	void UpperedMotionWarpSet() const;


// Flag
private:
	bool bIsUppered{ false };
	bool bIsUpperedHit{ false };
};
