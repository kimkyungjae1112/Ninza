// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NICameraInterface.generated.h"

class UCameraComponent;
class USpringArmComponent;

UINTERFACE(MinimalAPI)
class UNICameraInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INICameraInterface
{
	GENERATED_BODY()

public:
	virtual UCameraComponent* GetPlayerCamera() const = 0;
	virtual USpringArmComponent* GetPlayerSpringArm() const = 0;
};
