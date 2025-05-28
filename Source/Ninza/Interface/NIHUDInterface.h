// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NIHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNIHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INIHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetHUD(class UNIHUDWidget* InHUDWidget) = 0;
};
