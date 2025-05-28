// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NIMovementComponent.generated.h"

class ACharacter;
class UCharacterMovementComponent;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NINZA_API UNIMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNIMovementComponent();

	const FVector2D GetMoveInput() const;

// Movement
public:
	void Move(const FInputActionValue& Value);
	void UnMove();
	void Look(const FInputActionValue& Value);
	void Run();
	void Walk();

protected:
	virtual void BeginPlay() override;

// Owner
private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Player;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<UCharacterMovementComponent> MovementComp;

private:
	FVector2D InputValue;
};
