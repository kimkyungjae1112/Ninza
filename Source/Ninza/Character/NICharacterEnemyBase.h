// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NICharacterBase.h"
#include "Interface/NIAIInterface.h"
#include "Interface/NIOutlineInterface.h"
#include "NICharacterEnemyBase.generated.h"

UCLASS()
class NINZA_API ANICharacterEnemyBase 
	: public ANICharacterBase
	, public INIAIInterface
	, public INIOutlineInterface
{
	GENERATED_BODY()
	
public:
	ANICharacterEnemyBase();

	/* INIAIInterface Implement */
	virtual float GetTurnSpeed() const override;
	virtual float GetAttakRange() const override;

	virtual void SetAIAttackOnWalkDelegate(const FAttackOnWalkFinished& InAttackOnWalkFinished) override;

	virtual void AttackOnWalkByAI() override;

	// Super 호출 X
	virtual AAIController* GetAIController() const override { return nullptr; };

	/* INIClassIdentifierInterface Implement */
	virtual	FName GetClassName() const override;

	/* INIOutlineInterface Implement */
	virtual void SetOverlayMaterial() const override;
	virtual void SetUnOverlayMaterial() const override;

	/* AActor override */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	FAttackOnWalkFinished AttackOnWalkFinished;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstance> OutlineMaterial;

};
