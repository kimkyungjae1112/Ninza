// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NIAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UNIAnimInstance::UNIAnimInstance()
{
	MovingThreshold = 3.f;
	JumpingThreshold = 30.f;
	
}

void UNIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ACharacter>(GetOwningActor());
	if (Player)
	{
		Movement = Player->GetCharacterMovement();
	}
}

void UNIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size();
		Angle = UKismetMathLibrary::InverseTransformDirection(Player->GetActorTransform(), Velocity);
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z >= JumpingThreshold);
	}
}
