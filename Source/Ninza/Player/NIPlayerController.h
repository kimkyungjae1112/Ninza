// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NIPlayerController.generated.h"

class UNIHUDWidget;

UCLASS()
class NINZA_API ANIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ANIPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UNIHUDWidget> HUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNIHUDWidget> HUDWidgetPtr;
};
