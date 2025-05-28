// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/NIAnimMontageDataTable.h"
#include "NIEnemyAttackComponent.generated.h"

class UAnimInstance;
class UAnimMontage;
class ACharacter;

DECLARE_DELEGATE(FAttackOnWalkFinish)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINZA_API UNIEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNIEnemyAttackComponent();


// Attack Interface
public:
	void BeginAttackOnWalk();

	FAttackOnWalkFinish AttackOnWalkFinish;

// Attack Implement
protected:
	void EndAttackOnWalk(UAnimMontage* Target, bool IsProperlyEnd);


protected:
	virtual void BeginPlay() override;



// Owner
private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<UAnimInstance> Anim;

// Animation Data
private:
	UAnimMontage* GetAttackOnWalkMontage() const;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UDataTable> MoveTable;

	FNIAnimMontageDataTable MontageData;
		
};
