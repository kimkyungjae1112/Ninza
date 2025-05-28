// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NIPlayerController.h"
#include "UI/NIHUDWidget.h"

ANIPlayerController::ANIPlayerController()
{
	static ConstructorHelpers::FClassFinder<UNIHUDWidget> HUDWidgetClassRef(TEXT("/Game/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
}

void ANIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidgetPtr = CreateWidget<UNIHUDWidget>(this, HUDWidgetClass);
	if (HUDWidgetPtr)
	{
		HUDWidgetPtr->AddToViewport();
	}
}
