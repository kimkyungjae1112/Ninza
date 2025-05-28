// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NIAnimInstance.generated.h"

class ACharacter;
class UCharacterMovementComponent;
UCLASS()
class NINZA_API UNIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UNIAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	uint8 bIsDoubleJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	uint8 bIsDefense : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float JumpingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	FVector Angle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	FVector Velocity;


private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Player;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<UCharacterMovementComponent> Movement;

	
};
