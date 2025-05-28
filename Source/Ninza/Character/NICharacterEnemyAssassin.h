// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NICharacterEnemyBase.h"
#include "NICharacterEnemyAssassin.generated.h"

class UNIEnemyAttackComponent;
class UNIStatBaseComponent;
class UNIDamagedComponent;
class ANIAIControllerAssassin;

UCLASS()
class NINZA_API ANICharacterEnemyAssassin : public ANICharacterEnemyBase
{
	GENERATED_BODY()

public:
	ANICharacterEnemyAssassin();

	/* INIAIInterface Implement */
	virtual float GetTurnSpeed() const override;
	virtual float GetAttakRange() const override;
	virtual void AttackOnWalkByAI() override;
	
	// Super 호출 X
	virtual AAIController* GetAIController() const override;

	/* AActor override */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

// 유틸리티
private:
	ANIAIControllerAssassin* GetMyAIController() const;

// 공격 끝 판정
private:
	void EndAttackOnWalkByAI();

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIEnemyAttackComponent> AttackComp;
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIStatBaseComponent> StatComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNIDamagedComponent> DamagedComp;

// Weapon
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComp;
};
