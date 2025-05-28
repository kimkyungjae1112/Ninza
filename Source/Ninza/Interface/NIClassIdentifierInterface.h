// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NIClassIdentifierInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNIClassIdentifierInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INIClassIdentifierInterface
{
	GENERATED_BODY()

public:
	virtual	FName GetClassName() const = 0;

};
