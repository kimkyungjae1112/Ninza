// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NICharacterBase.h"
#include "Interface/NISwordInterface.h"
#include "Interface/NIHUDInterface.h"
#include "Interface/NICameraInterface.h"
#include "NICharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNIInputData;
class UNIMovementComponent;
class UNIAttackComponent;
class UNIStatBaseComponent;
class UNIHUDWidget;
class UNICameraData;

class APlayerController;

struct FInputActionValue;

DECLARE_DELEGATE(FOnInputChange)

USTRUCT()
struct FOnInputChangeWrapper
{
	GENERATED_BODY()

public:
	FOnInputChangeWrapper() {}
	FOnInputChangeWrapper(const FOnInputChange& InOnInputChange) : OnInputChange(InOnInputChange) {}

	FOnInputChange OnInputChange;
};

UCLASS()
class NINZA_API ANICharacterPlayer 
	: public ANICharacterBase
	, public INISwordInterface
	, public INIHUDInterface
	, public INICameraInterface
{
	GENERATED_BODY()
	
public:
	ANICharacterPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* INIClassIdentifierInterface Implement */
	virtual	FName GetClassName() const override;

	/* INISwordInterface Implement */
	virtual USkeletalMeshComponent* GetSwordComp() const override;
	virtual TMap<EBehaviorState, float> GetSwordDamage() const override;
	virtual EBehaviorState GetPlayerState() const override;

	/* INIHUDInterface Implement */
	virtual void SetHUD(UNIHUDWidget* InHUDWidget) override;
	
	/* INICameraInterface Implement */
	virtual UCameraComponent* GetPlayerCamera() const override;
	virtual USpringArmComponent* GetPlayerSpringArm() const override;

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

// 유틸리티 함수
private:
	APlayerController* GetPlayerController() const;

// 상태 변화
private:
	void SetBehaviorState(const EBehaviorState& InState);

	void SetIdleState();
	void SetCrouchState();
	void SetWalkState();
	void SetRunState();
	void SetJumpState();
	void SetAttackingState();
	void SetDefensingState();
	void SetFocusingState();

	UPROPERTY(EditDefaultsOnly, Category = "State")
	EBehaviorState CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "State")
	TMap<EBehaviorState, FOnInputChangeWrapper> InputChangeMap;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIMovementComponent> NinzaMovementComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIAttackComponent> AttackComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIStatBaseComponent> StatComp;

	// 임시
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TMap<EBehaviorState, float> SwordDamage;

// Input
private:
	/* Movement */
	void Move(const FInputActionValue& Value);
	void UnMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run();
	void Walk();
	void Jump();

	/* Attack */
	void Attacking();
	void AttackOnIdle();
	void AttackOnWalk();
	void AttackOnRun();
	void AttackOnFocusing();
	void RushAttack();

	void Defense();
	void UnDefense();

	void FocusingOn();

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UNIInputData> InputData;

	int32 MaxJumpCount{ 2 };
	int32 CurrentJumpCount{ 0 };


// Weapon
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComp;

// 카메라
private:
	void IdleToFocusOnCamera();
	void FocusOnToIdleCamera();

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

// UI
private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UNIHUDWidget> HUDWidget;

// Data
private:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<UNICameraData> IdleCameraSetData;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<UNICameraData> FocusOnCameraSetData;

// Flag 
private:
	void UnFocusing(const EBehaviorState& InBehaviorState);

	bool bIsAttacking{ false };
	bool bIsJumping{ false };
	bool bIsDefensing{ false };
	bool bIsFocusing{ false };
};
