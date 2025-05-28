// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/NIAnimMontageDataTable.h"
#include "Character/NICharacterPlayerState.h"
#include "NIAttackComponent.generated.h"

class UAnimInstance;
class UAnimMontage;
class UNIComboAttackData;
class UMotionWarpingComponent;

class ACharacter;
class APlayerController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEndAttack, const EBehaviorState&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINZA_API UNIAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNIAttackComponent();

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE bool GetFocusState() const { return bIsFocuesd; }


// Attack Interface
public:
	void AttackOnIdle();
	void AttackOnWalk();
	void AttackOnRun();
	void AttackOnFocusing();
	void RushAttack();
	FOnEndAttack OnEndAttack;


	// Defense
	void Defense();
	void UnDefense();

	// Focus
	void FocusingOn();
	void UnFocusingOn();

protected:
	virtual void BeginPlay() override;

// Utility 
private:
	APlayerController* GetPlayerController() const;

// Attack Implement
private:
	// Idle State Attack
	void BeginAttackOnIdle();
	void EndAttackOnIdle(UAnimMontage* Target, bool IsProperlyEnded);
	void SetComboCheckTimerOnIdle();
	void ComboCheckOnIdle();

	int32 CurrentComboOnIdle{ 0 };
	FTimerHandle ComboTimerHandleOnIdle;
	bool HasNextComboAttackOnIdle{ false };

	// Walk State Attack
	void BeginAttackOnWalk();
	void EndAttackOnWalk(UAnimMontage* Target, bool IsProperlyEnded);
	void SetComboCheckTimerOnWalk();
	void ComboCheckOnWalk();

	int32 CurrentComboOnWalk{ 0 };
	FTimerHandle ComboTimerHandleOnWalk;
	bool HasNextComboAttackOnWalk{ false };

	// Run State Attack
	void BeginAttackOnRun();
	void EndAttackOnRun(UAnimMontage* Target, bool IsProperlyEnded);
	void SetComboCheckTimerOnRun();
	void ComboCheckOnRun();

	int32 CurrentComboOnRun{ 0 };
	FTimerHandle ComboTimerHandleOnRun;
	bool HasNextComboAttackOnRun{ false };

	// Focusing State Attack
	void BeginAttackOnFocusing();
	void EndAttackOnFocusing(UAnimMontage* Target, bool IsProperlyEnded);
	void SetComboCheckTimerOnFocusing();
	void ComboCheckOnFocusing();

	int32 CurrentComboOnFocusing{ 0 };
	FTimerHandle ComboTimerHandleOnFocusing;
	bool HasNextComboAttackOnFocusing{ false };

	// Rush Attack
	void EndRushAttack(UAnimMontage* Target, bool IsProperlyEnded);

// Owner
private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Player;
	
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<UAnimInstance> Anim;

// Animation
private:
	UAnimMontage* GetMontageAttackOnIdle() const;
	UAnimMontage* GetMontageAttackOnWalk() const;
	UAnimMontage* GetMontageAttackOnRun() const;
	UAnimMontage* GetMontageAttackOnFocusing() const;
	UAnimMontage* GetMontageRushAttack() const;

	UPROPERTY(VisibleAnywhere, Category = "DataTable")
	TObjectPtr<UDataTable> MontageTable;

	FNIAnimMontageDataTable MontageData;

// Combo Attack Data
private:
	UPROPERTY(VisibleAnywhere, Category = "Combo")
	TObjectPtr<UNIComboAttackData> ComboData;

// Focus Attack
private:
	UPROPERTY(VisibleAnywhere, Category = "Focus Attack")
	TObjectPtr<AActor> FocusedTarget;

	float FocusDistance{ 2000.f };
	bool bIsFocuesd{ false };

// MotionWarping
private:
	UMotionWarpingComponent* GetMotionWarpComp() const;

	void SetRushAttackMotionWarpSet() const;
};
