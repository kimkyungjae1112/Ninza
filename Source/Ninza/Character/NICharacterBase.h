// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/NIClassIdentifierInterface.h"
#include "NICharacterBase.generated.h"

class UMotionWarpingComponent;

UCLASS()
class NINZA_API ANICharacterBase 
	: public ACharacter
	, public IGenericTeamAgentInterface
	, public INIClassIdentifierInterface
{
	GENERATED_BODY()

public:
	ANICharacterBase();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

	/* INIClassIdentifierInterface Implement */
	virtual	FName GetClassName() const override { return ClassName; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Name")
	FName ClassName;

	FGenericTeamId TeamId;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UMotionWarpingComponent> MotionWarpComp;
};
