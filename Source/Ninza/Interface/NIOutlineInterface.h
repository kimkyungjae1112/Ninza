// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NIOutlineInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNIOutlineInterface : public UInterface
{
	GENERATED_BODY()
};

class NINZA_API INIOutlineInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlayMaterial() const = 0;
	virtual void SetUnOverlayMaterial() const = 0;
};
