// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Character/NIMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"

UNIMovementComponent::UNIMovementComponent()
{
}

const FVector2D UNIMovementComponent::GetMoveInput() const
{
	return InputValue;
}

void UNIMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	if (Player)
	{
		MovementComp = Player->GetCharacterMovement();

		check(MovementComp);
	}
}

void UNIMovementComponent::Move(const FInputActionValue& Value)
{
	InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Player->GetController()->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			
	Player->AddMovementInput(ForwardVector, InputValue.X);
	Player->AddMovementInput(RightVector, InputValue.Y);
}

void UNIMovementComponent::UnMove()
{
	InputValue = FVector2D::ZeroVector;
}

void UNIMovementComponent::Look(const FInputActionValue& Value)
{
	FVector2D InputValueLook = Value.Get<FVector2D>();

	Player->AddControllerPitchInput(-InputValueLook.Y);
	Player->AddControllerYawInput(InputValueLook.X);
}

void UNIMovementComponent::Run()
{
	MovementComp->MaxWalkSpeed = 800.f;
}

void UNIMovementComponent::Walk()
{
	MovementComp->MaxWalkSpeed = 500.f;
}



