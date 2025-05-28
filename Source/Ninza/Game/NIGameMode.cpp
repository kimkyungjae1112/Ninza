// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/NIGameMode.h"
#include "GameFramework/PlayerController.h"

ANIGameMode::ANIGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/Ninza.NIPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprint/Character/BP_CharacterPlayer.BP_CharacterPlayer_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
}
